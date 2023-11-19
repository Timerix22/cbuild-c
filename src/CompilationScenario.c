#include <unistd.h>
#include "CompilationScenario.h"
#include "../kerep/src/Filesystem/filesystem.h"
#include "process/Process.h"

kt_define(Language, NULL, NULL);
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

void Tool_construct(Tool* ptr, Autoarr(Pointer)* aliases, const char* exe_file, bool parallel, Autoarr(Pointer)* supported_languages){
    ptr->aliases = aliases;
    ptr->exe_file = exe_file;
    ptr->parallel = parallel;
    ptr->supported_languages = Hashtable_create();
    Autoarr_foreach(supported_languages, _l,
        Language* l = _l;
        Autoarr_foreach(l->aliases, l_name, 
            Hashtable_add(ptr->supported_languages, l_name, UniHeapPtr(Language, l)));
    )
    ptr->src_languages = Hashtable_create();
    ptr->src_dirs = Autoarr_create(Pointer, 32, 32);
    ptr->pre_args = Autoarr_create(Pointer, 32, 32);
    ptr->post_args = Autoarr_create(Pointer, 32, 32);
}

void Tool_destruct(Tool* ptr){
    // TODO
}

void Language_construct(Language* ptr, Autoarr(Pointer)* aliases, Autoarr(Pointer)* file_extensions){
    ptr->aliases = aliases;
    ptr->file_extensions = file_extensions;
}

void Language_destruct(Language* ptr){
    // TODO
}

#define Dtsod_getStrField(FIELD, OBJ) \
    if(Hashtable_tryGet(dtsod, #FIELD, &val)){ \
        if(!UniCheckTypePtr(val, char)) \
            safethrow(#FIELD " value expected to be string",;);\
        OBJ->FIELD = val.VoidPtr; \
    }

#define Dtsod_addToStrAr(FIELD, OBJ) \
    if(Hashtable_tryGet(dtsod, #FIELD, &val)){ \
        if(!UniCheckTypePtr(val, Autoarr(Unitype))) \
            safethrow(#FIELD " value expected to be a string array", ;); \
        Autoarr(Unitype)* ar = val.VoidPtr; \
        Autoarr_foreach(ar, el, \
            if(!UniCheckTypePtr(el, char)) \
                safethrow(#FIELD " array values expected to be string", ;); \
            Autoarr_add(OBJ->FIELD, el.VoidPtr); \
        ) \
    }

Maybe Tool_tryApplyOptions(Tool* t, Hashtable* dtsod){
    Unitype val = UniNull;
    Dtsod_addToStrAr(src_dirs, t);
    Dtsod_addToStrAr(pre_args, t);
    Dtsod_addToStrAr(post_args, t);
    if(Hashtable_tryGet(dtsod, "src_languages", &val)){
        if(!UniCheckTypePtr(val, Autoarr(Unitype)))
            safethrow("src_languages value expected to be a string array", ;);
        Autoarr(Unitype)* ar = val.VoidPtr;
        Autoarr_foreach(ar, el,
            if(!UniCheckTypePtr(el, char))
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

    // adds tools to tool_order
    Dtsod_addToStrAr(tool_order, sc);

    // sets options for each tool
    try(CompilationScenario_tryApplyPlatformSpecificOptions(sc, dtsod), _m0, ;);
    Hashtable_foreach(sc->tools, _tool,
        Tool* tool = _tool.value.VoidPtr;
        if(Hashtable_tryGet(dtsod, _tool.key, &val)){
            if(!UniCheckTypePtr(val, Hashtable))
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
        if(!UniCheckTypePtr(val, Hashtable))
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

Maybe CompilationScenario_tryRegisterLanguages(CompilationScenario* sc, Hashtable* dtsod){
    Unitype val = UniNull;
    if(!Hashtable_tryGet(dtsod, "languages", &val))
        return SUCCESS(UniFalse);

    if(!UniCheckTypePtr(val, Hashtable))
        safethrow(ERR_WRONGTYPE, ;);
    Autoarr(Unitype)* languages_serializad = val.VoidPtr;
    
    Autoarr_foreach(languages_serializad, ldtsod,
        if(!UniCheckTypePtr(ldtsod, Hashtable))
            safethrow(ERR_WRONGTYPE, ;);
        
        // reads aliases: string[] from dtsod
        if(!Hashtable_tryGet(ldtsod.VoidPtr, "aliases", &val))
            safethrow(ERR_FORMAT, ;);
        if(!UniCheckTypePtr(val, Autoarr(Unitype)))
            safethrow(ERR_WRONGTYPE, ;);
        Autoarr(Unitype)* aliases_uni = val.VoidPtr;
        Autoarr(Pointer)* aliases = Autoarr_create(Pointer, 32, 32);
        Autoarr_foreach(aliases_uni, au, 
            if(!UniCheckTypePtr(au, char))
                safethrow(ERR_WRONGTYPE, ;);
            Autoarr_add(aliases, au.VoidPtr);
        )

        // reads file_extensions: string[] from dtsod
        if(!Hashtable_tryGet(ldtsod.VoidPtr, "file_extensions", &val))
            safethrow(ERR_FORMAT, ;);
        if(!UniCheckTypePtr(val, Autoarr(Unitype)))
            safethrow(ERR_WRONGTYPE, ;);
        Autoarr(Unitype)* file_extensions_uni = val.VoidPtr;
        Autoarr(Pointer)* file_extensions = Autoarr_create(Pointer, 32, 32);
        Autoarr_foreach(file_extensions_uni, feu, 
            if(!UniCheckTypePtr(feu, char))
                safethrow(ERR_WRONGTYPE, ;);
            Autoarr_add(file_extensions, feu.VoidPtr);
        )

        Language* lang = malloc(sizeof(Language));
        Language_construct(lang, aliases, file_extensions);
        // registers each alias of the language
        Autoarr_foreach(aliases, l_name,
            if(!Hashtable_tryAdd(sc->languages, l_name, UniHeapPtr(Language, lang)))
                safethrow(cptr_concat("language '", l_name, "has been already registered"), ;);
        )
    )

    return SUCCESS(UniTrue);
}

Maybe CompilationScenario_tryRegisterTools(CompilationScenario* sc, Hashtable* dtsod){
    Unitype val = UniNull;
    if(!Hashtable_tryGet(dtsod, "tools", &val))
        return SUCCESS(UniFalse);

    if(!UniCheckTypePtr(val, Hashtable))
        safethrow(ERR_WRONGTYPE, ;);
    Autoarr(Unitype)* tools_serializad = val.VoidPtr;
    
    Autoarr_foreach(tools_serializad, tdtsod,
        if(!UniCheckTypePtr(tdtsod, Hashtable))
            safethrow(ERR_WRONGTYPE, ;);

        // reads exe_file: string from dtsod
        if(!Hashtable_tryGet(tdtsod.VoidPtr, "exe_file", &val))
            safethrow(ERR_FORMAT, ;);
        if(!UniCheckTypePtr(val, char))
            safethrow(ERR_WRONGTYPE, ;);
        char* exe_file = val.VoidPtr;

        // reads parallel: bool from dtsod
        if(!Hashtable_tryGet(tdtsod.VoidPtr, "parallel", &val))
            safethrow(ERR_FORMAT, ;);
        if(!UniCheckType(val, bool))
            safethrow(ERR_WRONGTYPE, ;);
        bool parallel = val.Bool;

        // reads aliases: string[] from dtsod
        if(!Hashtable_tryGet(tdtsod.VoidPtr, "aliases", &val))
            safethrow(ERR_FORMAT, ;);
        if(!UniCheckTypePtr(val, Autoarr(Unitype)))
            safethrow(ERR_WRONGTYPE, ;);
        Autoarr(Unitype)* aliases_uni = val.VoidPtr;
        Autoarr(Pointer)* aliases = Autoarr_create(Pointer, 32, 32);
        Autoarr_foreach(aliases_uni, au, 
            if(!UniCheckTypePtr(au, char))
                safethrow(ERR_WRONGTYPE, ;);
            Autoarr_add(aliases, au.VoidPtr);
        )

        // reads supported_languages: string[] dtsod
        if(!Hashtable_tryGet(tdtsod.VoidPtr, "supported_languages", &val))
            safethrow(ERR_FORMAT, ;);
        if(!UniCheckTypePtr(val, Autoarr(Unitype)))
            safethrow(ERR_WRONGTYPE, ;);
        Autoarr(Unitype)* supported_languages_uni = val.VoidPtr;
        Autoarr(Pointer)* supported_languages = Autoarr_create(Pointer, 32, 32);
        Autoarr_foreach(supported_languages_uni, lu, 
            if(!UniCheckTypePtr(lu, char))
                safethrow(ERR_WRONGTYPE, ;);
            char* l_name = lu.VoidPtr;
            // gets language pointer from CompilationScenario regisgered languages
            if(!Hashtable_tryGet(sc->languages, l_name, &val))
                safethrow(ERR_KEYNOTFOUND, ;);
            if(!UniCheckTypePtr(val, Language))
                safethrow(ERR_WRONGTYPE, ;);
            Language* lang = val.VoidPtr;
            Autoarr_add(supported_languages, lang);
        )

        Tool* tool = malloc(sizeof(Tool));
        Tool_construct(tool, aliases, exe_file, parallel, supported_languages);
        // registers each alias of the tool
        Autoarr_foreach(aliases, t_name,
            if(!Hashtable_tryAdd(sc->tools, t_name, UniHeapPtr(Tool, tool)))
                safethrow(cptr_concat("tool '", t_name, "has been already registered"), ;);
        )
    )

    return SUCCESS(UniTrue);
}

Maybe CompilationScenario_applyProjectOptions(CompilationScenario* sc, Hashtable* dtsod, const char* configuration, const char* task){
    // TODO version check
    // TODO import
    try(CompilationScenario_tryRegisterLanguages(sc, dtsod), _m05, ;);
    try(CompilationScenario_tryRegisterTools(sc, dtsod), _m06, ;);
    // project-wide options
    try(CompilationScenario_tryApplyToolsOptions(sc, dtsod), _m0, ;);
    // configuration options
    try(CompilationScenario_applyConfigurationOptions(sc, dtsod, configuration), _m1, ;);
    // task options
    try(CompilationScenario_applyTaskOptions(sc, dtsod, task), _m2, ;);
    return MaybeNull;
}

Maybe Tool_exec(Tool* tool){
    Autoarr(Pointer)* args_ar = Autoarr_create(Pointer, 32, 32);
    Autoarr_foreach(tool->pre_args, arg, 
        Autoarr_add(args_ar, arg));

    Autoarr(Pointer)* sources = Autoarr_create(Pointer, 32, 32);
    Autoarr_foreach(tool->src_dirs, dir,
        try(dir_getFiles(dir, true), _m2, ;);
        char** files = _m2.value.VoidPtr;
        while(*files){
            Autoarr_add(sources, *files);
            files++;
        }
    );

    if(tool->parallel){
        safethrow(ERR_NOTIMPLEMENTED, ;);
    }
    else {
        Autoarr_foreach(sources, file, 
            Autoarr_add(args_ar, file));
    }

    Autoarr_foreach(tool->post_args, arg, 
        Autoarr_add(args_ar, arg));

    const char** args = (const char**)Autoarr_toArray(args_ar);
    i32 argc = Autoarr_length(args_ar);
    Autoarr_freeWithoutMembers(args_ar, true);

    Process tool_proc;
    try(process_start(&tool_proc, tool->exe_file, args, argc, true), _m5512, Autoarr_freeWithoutMembers(sources, true))

    // TODO wrap tool_proc->io    
    process_waitForExit(&tool_proc);

    Autoarr_freeWithoutMembers(sources, true);
    return MaybeNull;
}

Maybe CompilationScenario_exec(CompilationScenario* sc){
    Autoarr_foreach(sc->tool_order, tool_name,
        kprintf("tool: '%s'\n", tool_name);
        Unitype uni;
        if(!Hashtable_tryGet(sc->tools, tool_name, &uni))
            safethrow(ERR_KEYNOTFOUND, ;);
        if(!UniCheckTypePtr(uni, Tool))
            safethrow(ERR_WRONGTYPE, ;);
        Tool* tool = uni.VoidPtr;
        try(Tool_exec(tool), _m1, ;);    
    )
    return MaybeNull;
}
