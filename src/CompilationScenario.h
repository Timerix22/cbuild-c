#include "../kerep/src/DtsodParser/DtsodV24.h"

extern const char* os;
extern const char* arch;

STRUCT(Language,
    Autoarr(Pointer)* aliases;
    Autoarr(Pointer)* file_extensions;
)

STRUCT(Tool,
    Autoarr(Pointer)* aliases;
    const char* exe_file;
    bool parallel;
    Hashtable* supported_languages;
    Hashtable* src_languages;
    Autoarr(Pointer)* src_dirs;
    Autoarr(Pointer)* pre_args;
    Autoarr(Pointer)* post_args;
)

STRUCT(CompilationScenario,
    Hashtable* tools; /* Hashtable<Tool> */
    Hashtable* languages; /* Hashtable<Languages> */
    Autoarr(Pointer)* tool_order; /* Autoarr(char[]) */
)

/*         Public Functions         */

void CompilationScenario_construct(CompilationScenario* ptr);

void CompilationScenario_destruct(CompilationScenario* ptr);

/// applies all options from project, selected configuration and task
///@return Maybe<void>
Maybe CompilationScenario_applyProjectOptions(CompilationScenario* sc, Hashtable* dtsod, const char* configuration, const char* task);

/// compiles project using given scenario
/// @return Maybe<void>
Maybe CompilationScenario_exec(CompilationScenario* sc);


/*        Internal Functions        */

/// tries to register proramming languages from dtsod field "languages"
///@return Maybe<bool>
Maybe CompilationScenario_tryRegisterLanguages(CompilationScenario* sc, Hashtable* dtsod);

/// tries to register tools from dtsod field "tools"
///@return Maybe<bool>
Maybe CompilationScenario_tryRegisterTools(CompilationScenario* sc, Hashtable* dtsod);

/// tries to set options for tools registered in the project
///@return Maybe<bool>
Maybe CompilationScenario_tryApplyToolsOptions(CompilationScenario* sc, Hashtable* dtsod);

/// tries to get any options from field <condition_name>
///@return Maybe<bool>
Maybe CompilationScenario_tryApplyConditionalOptions(CompilationScenario* sc, Hashtable* dtsod, const char* condition_name);

/// tries to get options from dtsod fields named "windowss", "linux", "android", "x64", "android-arm32", "windows_x86", etc.
///@return Maybe<bool>
Maybe CompilationScenario_tryApplyPlatformSpecificOptions(CompilationScenario* sc, Hashtable* dtsod);

///@return Maybe<void>
Maybe CompilationScenario_applyConfigurationOptions(CompilationScenario* sc, Hashtable* dtsod, const char* configuration);

///@return Maybe<void>
Maybe CompilationScenario_applyTaskOptions(CompilationScenario* sc, Hashtable* dtsod, const char* task);
