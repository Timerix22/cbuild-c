#include <unistd.h>
#include "CompilationScenario.h"

kt_define(Language, NULL, NULL);
Autoarr_define(Language, false);
kt_define(Tool, NULL, NULL);
kt_define(CompilationScenario, (freeMembers_t)CompilationScenario_destruct, NULL);

void CompilationScenario_construct(CompilationScenario* ptr){
    ptr->languages = Hashtable_create();
    ptr->tools = Hashtable_create();
    ptr->tool_order = Autoarr_create(Pointer, 32, 32);
}

void CompilationScenario_destruct(CompilationScenario* ptr){
    // TODO
}

void Tool_construct(Tool* ptr, Autoarr(Pointer)* aliases, const char* exe_file, bool parallel, Autoarr(Language)* supported_languages){
    ptr->aliases = aliases;
    ptr->exe_file = exe_file;
    ptr->parallel = parallel;
    ptr->supported_languages = Hashtable_create();
    Autoarr_foreach(supported_languages, l,
        Language* l_copy = malloc(sizeof(Language));
        *l_copy = l;
        Autoarr_foreach(l.aliases, l_name, 
            Hashtable_add(ptr->supported_languages, l_name, UniHeapPtr(Language, l_copy)));
    )
    ptr->src_languages = Hashtable_create();
    ptr->src_dirs = Autoarr_create(Pointer, 32, 32);
    ptr->pre_args = Autoarr_create(Pointer, 32, 32);
    ptr->post_args = Autoarr_create(Pointer, 32, 32);
}

void Tool_destruct(Tool* ptr){
    // TODO
}

#define Dtsod_getStrField(FIELD, OBJ) \
    if(Hashtable_tryGet(dtsod, #FIELD, &val)){ \
        if(val.typeId != ktid_char_Ptr) \
            safethrow(#FIELD " value expected to be string",;);\
        OBJ->FIELD = val.VoidPtr; \
    }

#define Dtsod_addToStrAr(FIELD, OBJ) \
    if(Hashtable_tryGet(dtsod, #FIELD, &val)){ \
        if(val.typeId != ktid_Autoarr_Unitype_Ptr) \
            safethrow(#FIELD " value expected to be a string array", ;); \
        Autoarr(Unitype)* ar = val.VoidPtr; \
        Autoarr_foreach(ar, el, \
            if(el.typeId != ktid_char_Ptr) \
                safethrow(#FIELD " array values expected to be string", ;); \
            Autoarr_add(OBJ->FIELD, el.VoidPtr); \
        ) \
    }

Maybe Tool_tryApplyOptions(Tool* t, Hashtable* dtsod){
    Unitype val = UniNull;
    Dtsod_addToStrAr(src_dirs, t);
    Dtsod_addToStrAr(pre_args, t);
    Dtsod_addToStrAr(post_args, t);
    if(Hashtable_tryGet(dtsod, "src_languages", &val)){ \
        if(val.typeId != ktid_Autoarr_Unitype_Ptr) \
            safethrow("src_languages value expected to be a string array", ;); \
        Autoarr(Unitype)* ar = val.VoidPtr;
        Autoarr_foreach(ar, el,
            if(el.typeId != ktid_char_Ptr)
                safethrow("src_languages array values expected to be string", ;);
            const char* l_name = el.VoidPtr;
            if(!Hashtable_tryGet(t->supported_languages, l_name, &val))
                safethrow(cptr_concat("language '", l_name, "' isn't supported by tool '", Autoarr_get(t->aliases, 0), "'"), ;);
            Hashtable_add(t->src_languages, l_name, val);
        )
    }
    return MaybeNull;
}

Maybe CompilationScenario_tryApplyToolsOptions(CompilationScenario* sc, Hashtable* dtsod){
    Unitype val = UniNull;

    try(CompilationScenario_tryApplyPlatformSpecificOptions(sc, dtsod), _m0, ;);
    Hashtable_foreach(sc->tools, _tool,
        Tool* tool = _tool.value.VoidPtr;
        if(Hashtable_tryGet(dtsod, _tool.key, &val)){
            if(val.typeId != ktid_ptrName(Hashtable))
                safethrow(ERR_WRONGTYPE, ;);
            Hashtable* tool_dtsod = val.VoidPtr;
            try(Tool_tryApplyOptions(tool, tool_dtsod), _m1, ;);
        }
    )

    return SUCCESS(UniBool(Unitype_isUniNull(val) || _m0.value.Bool));
}

Maybe CompilationScenario_tryApplyConditionalOptions(CompilationScenario* sc, Hashtable* dtsod, const char* condition_name){
    Unitype val = UniNull;
    if(Hashtable_tryGet(dtsod, condition_name, &val)){
        if(val.typeId != ktid_Hashtable_Ptr)
            safethrow(cptr_concat(condition_name, " expected to be key-value map"), ;);
        Hashtable* conditional_options = val.VoidPtr;
        try(CompilationScenario_tryApplyToolsOptions(sc, conditional_options), _m0, ;);
        return SUCCESS(UniTrue);
    }
    else return SUCCESS(UniFalse);
}

Maybe CompilationScenario_tryApplyPlatformSpecificOptions(CompilationScenario* sc, Hashtable* dtsod){
    try(CompilationScenario_tryApplyConditionalOptions(sc, dtsod, os), _m0, ;);
    try(CompilationScenario_tryApplyConditionalOptions(sc, dtsod, arch), _m1, ;);
    
    char* os_and_arch = cptr_concat(os, "-", arch);
    try(CompilationScenario_tryApplyConditionalOptions(sc, dtsod, os_and_arch), _m2, ;);
    free(os_and_arch);

    os_and_arch = cptr_concat(os, "_", arch);
    try(CompilationScenario_tryApplyConditionalOptions(sc, dtsod, os_and_arch), _m3, ;);
    free(os_and_arch);

    return SUCCESS(UniBool(_m0.value.Bool || _m1.value.Bool || _m2.value.Bool || _m3.value.Bool));
}

Maybe CompilationScenario_applyConfigurationOptions(CompilationScenario* sc, Hashtable* dtsod, const char* configuration){
    try(CompilationScenario_tryApplyConditionalOptions(sc, dtsod, configuration), _m0, ;);
    if(!_m0.value.Bool)
        safethrow(cptr_concat("configuration '", configuration, "' not found"), ;);
    
    return MaybeNull;
}

Maybe CompilationScenario_applyTaskOptions(CompilationScenario* sc, Hashtable* dtsod, const char* task){
    try(CompilationScenario_tryApplyConditionalOptions(sc, dtsod, task), _m0, ;);
    if(!_m0.value.Bool)
        safethrow(cptr_concat("task '", task, "' not found"), ;);

    return MaybeNull;
}

Maybe CompilationScenario_applyProjectOptions(CompilationScenario* sc, Hashtable* dtsod, const char* configuration, const char* task){
    // TODO version check
    // TODO import
    // TODO register tools
    // TODO register languagess
    // project-wide options
    try(CompilationScenario_tryApplyToolsOptions(sc, dtsod), _m0, ;);
    // configuration options
    try(CompilationScenario_applyConfigurationOptions(sc, dtsod, configuration), _m1, ;);
    // task options
    try(CompilationScenario_applyTaskOptions(sc, dtsod, task), _m2, ;);
    return MaybeNull;
}

Maybe CompilationScenario_exec(CompilationScenario* sc){
    /*const char ** compiler_args;
    Autoarr_foreach(sc->sources, arg,
        int rzlt = -1;
        if(rzlt != 0){
            kprintf("\nprocess exited with code %i\n", rzlt);
            return false;
        }
    );*/
    return MaybeNull;
}
