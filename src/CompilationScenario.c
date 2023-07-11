#include "CompilationScenario.h"

void CompilationScenario_construct(CompilationScenario* ptr){
    ptr->compiler = "UNDEFINED_COMPILER";
    ptr->obj_dir = "obj";
    ptr->out_dir = "bin";
    ptr->args_pre = Autoarr_create(Pointer, 32, 32);
    ptr->sources = Autoarr_create(Pointer, 32, 32);
    ptr->args_post = Autoarr_create(Pointer, 32, 32);
    ptr->defines = Autoarr_create(Pointer, 32, 32);
}

void CompilationScenario_destruct(CompilationScenario* ptr){
    Autoarr_freeWithoutMembers(ptr->args_pre, true);
    Autoarr_freeWithoutMembers(ptr->sources, true);
    Autoarr_freeWithoutMembers(ptr->args_post, true);
    Autoarr_freeWithoutMembers(ptr->defines, true);
}

#define Dtsod_setStrField(FIELD) \
    if(Hashtable_tryGet(dtsod, #FIELD, &val)){ \
        if(val.typeId != ktid_char_Ptr) \
            safethrow(#FIELD " value expected to be string",;);\
        sc->FIELD = val.VoidPtr; \
    }
    
#define Dtsod_addArrField(FIELD, ELEM_TYPE) \
    if(Hashtable_tryGet(dtsod, #FIELD, &val)){ \
        if(val.typeId != ktid_Autoarr_Unitype_Ptr) \
            safethrow(#FIELD " value expected to be array", ;); \
        Autoarr(Unitype)* ar = val.VoidPtr; \
        Autoarr_foreach(ar, el, \
            if(el.typeId != ktid_ptrName(ELEM_TYPE)) \
                safethrow(#FIELD " array values expected to be " #ELEM_TYPE, ;); \
            Autoarr_add(sc->FIELD, el.VoidPtr); \
        ) \
    }

Maybe CompilationScenario_tryApplyOptions(CompilationScenario* sc, Hashtable* dtsod){
    Unitype val = UniNull;
    Dtsod_setStrField(compiler);
    Dtsod_setStrField(obj_dir);
    Dtsod_setStrField(out_dir);
    Dtsod_addArrField(args_pre, char);
    Dtsod_addArrField(sources, char);
    Dtsod_addArrField(args_post, char);
    Dtsod_addArrField(defines, char);

    try(CompilationScenario_tryApplyPlatformSpecificOptions(sc, dtsod), _m0, ;);

    return SUCCESS(UniBool(Unitype_isUniNull(val) || _m0.value.Bool));
}

Maybe CompilationScenario_tryApplyConditionalOptions(CompilationScenario* sc, Hashtable* dtsod, const char* condition_name){
    Unitype val = UniNull;
    if(Hashtable_tryGet(dtsod, condition_name, &val)){
        if(val.typeId != ktid_Hashtable_Ptr)
            safethrow(cptr_concat(condition_name, " expected to be key-value map"), ;);
        Hashtable* conditional_options = val.VoidPtr;
        try(CompilationScenario_tryApplyOptions(sc, conditional_options), _m0, ;);
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
    // general options
    try(CompilationScenario_tryApplyOptions(sc, dtsod), _m0, ;);
    // configuration options
    try(CompilationScenario_applyConfigurationOptions(sc, dtsod, configuration), _m1, ;);
    // task options
    try(CompilationScenario_applyTaskOptions(sc, dtsod, task), _m2, ;);
    return MaybeNull;
}
