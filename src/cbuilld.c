#include "../../GraphC/dependencies/kerep/src/base/base.h"
#include "../../GraphC/dependencies/kerep/src/Filesystem/filesystem.h"
#include "../../GraphC/dependencies/kerep/src/DtsodParser/DtsodV24.h"
#include "CompilationScenario.h"

#ifndef OS
#define OS "UNDEFINED"
#endif
#ifndef ARCH
#define ARCH "UNDEFINED"
#endif

const char* os=OS;
const char* arch=ARCH;

const char* global_out_dir="bin";
const char* configuration="release";
const char* task="exe";
const char** projects=NULL;
u16 project_count = 0;

int erri(ErrorId err_code){
    throw(err_code);
    return -1;
}

#define argIs(STR) cptr_equals(arg, STR)
#define argNext() argv[++argi < argc ? argi : erri(ERR_WRONGINDEX)]

char* projectFileFromDir(const char* dir){
    throw(ERR_NOTIMPLEMENTED);
    // tryLast(dir_getFiles(dir, false), _m, ;);
}

int main(const int argc, const char** argv){
    kt_beginInit();
    kt_initKerepTypes();
    kt_endInit();

    if(cptr_equals(os, "UNDEFINED"))
        throw("Operation system undefined. Recompile cbuild with flag -DOS=$(./detect_os.sh)");
    if(cptr_equals(arch, "UNDEFINED"))
        throw("CPU architecture undefined. Recompile cbuild with flag -DARCH=$(./detect_arch.sh)");

    Autoarr(Pointer)* projects_ar = Autoarr_create(Pointer, 16, 32);

    for(int argi = 1; argi < argc; argi++){
        const char* arg = argv[argi];
        kprintf("arg: %s\n", arg);
        if(argIs("-h") || argIs("--help") || argIs("/?"))
            kprintf("Usage: cbuild [options] [projects files/dirs]\n"
               "  Options:\n"
               "    -h, --help, /?          Display this message.\n"
               "    -o, --out-dir           Set global output directory (default=bin).\n"
               "    -c, --configuration     Select project configuration (default=release).\n"
               "    -t, --task              Select build task from project.\n");
        else if(argIs("-o") || argIs("--out-dir"))
            global_out_dir = argNext();
        else if(argIs("-c") || argIs("--configuration"))
            configuration = argNext();
        else if(argIs("-t") || argIs("--task"))
            task = argNext();
        else {
            if(arg[0] == '-')
                throw(cptr_concat("invalid argument: ", arg));
            Autoarr_add(projects_ar, arg);
        }
    }

    project_count = Autoarr_length(projects_ar);
    projects = (const char**)Autoarr_toArray(projects_ar);
    Autoarr_freeWithoutMembers(projects_ar, true);
    if(project_count == 0){
        projects = malloc(sizeof(char*));
        projects[0] = projectFileFromDir(".");
    }

    for(u16 i=0; i < project_count; i++){
        const char* proj = projects[i];
        const char* proj_file_path = NULL;
        if(file_exists(proj))
            proj_file_path = proj;
        else if(dir_exists(proj))
            proj_file_path = projectFileFromDir(proj);
        
        tryLast(file_open(proj_file_path, FileOpenMode_Read), _m1, ;);
        FileHandle proj_file = _m1.value.VoidPtr;
        char* proj_file_text;
        tryLast(file_readAll(proj_file, &proj_file_text), _m2, file_close(proj_file));
        file_close(proj_file);

        tryLast(DtsodV24_deserialize(proj_file_text), _m3, free(proj_file_text));
        Hashtable* proj_dtsod = _m3.value.VoidPtr;
        CompilationScenario proj_sc;
        CompilationScenario_construct(&proj_sc);

        tryLast(CompilationScenario_applyProjectOptions(&proj_sc, proj_dtsod, configuration, task), _m4, free(proj_file_text))

        CompilationScenario_destruct(&proj_sc);
        Hashtable_free(proj_dtsod);
        free(proj_file_text);
    }
    
    kt_free();
    return 0;
}
