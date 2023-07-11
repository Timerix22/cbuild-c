#include "../../GraphC/dependencies/kerep/src/DtsodParser/DtsodV24.h"

extern const char* os;
extern const char* arch;

typedef struct {
    const char* compiler;
    const char* obj_dir;
    const char* out_dir;
    Autoarr(Pointer)* args_pre;
    Autoarr(Pointer)* sources;
    Autoarr(Pointer)* args_post;
    Autoarr(Pointer)* defines;
} CompilationScenario;

/*         Public Functions         */

void CompilationScenario_construct(CompilationScenario* ptr);

void CompilationScenario_destruct(CompilationScenario* ptr);

/// applies all options from project, selected configuration and task
///@return Maybe<void>
Maybe CompilationScenario_applyProjectOptions(CompilationScenario* sc, Hashtable* dtsod, const char* configuration, const char* task);


/*        Internal Functions        */

///@return Maybe<bool>
Maybe CompilationScenario_tryApplyOptions(CompilationScenario* sc, Hashtable* dtsod);

///@return Maybe<bool>
Maybe CompilationScenario_tryApplyConditionalOptions(CompilationScenario* sc, Hashtable* dtsod, const char* condition_name);

/// tries to get options from dtsod fields named "windows", "linux", "android", "x64", "android-arm32", "windows_x86", etc.
///@return Maybe<bool>
Maybe CompilationScenario_tryApplyPlatformSpecificOptions(CompilationScenario* sc, Hashtable* dtsod);

///@return Maybe<void>
Maybe CompilationScenario_applyConfigurationOptions(CompilationScenario* sc, Hashtable* dtsod, const char* configuration);

///@return Maybe<void>
Maybe CompilationScenario_applyTaskOptions(CompilationScenario* sc, Hashtable* dtsod, const char* task);
