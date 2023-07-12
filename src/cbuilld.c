#include "../kerep/src/base/base.h"
#include "../kerep/src/Filesystem/filesystem.h"
#include "../kerep/src/DtsodParser/DtsodV24.h"
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
const char* project_dir_or_file="./";
Autoarr(Pointer)* tasks;

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

    tasks = Autoarr_create(Pointer, 16, 32);

    if(cptr_equals(os, "UNDEFINED"))
        throw("Operation system undefined. Recompile cbuild with flag -DOS=\\\"$(./detect_os.sh)\\\"");
    if(cptr_equals(arch, "UNDEFINED"))
        throw("CPU architecture undefined. Recompile cbuild with flag -DARCH=\\\"$(./detect_arch.sh)\\\"");

    for(int argi = 1; argi < argc; argi++){
        const char* arg = argv[argi];
        kprintf("arg: %s\n", arg);
        if(argIs("-h") || argIs("--help") || argIs("/?")){
            kprintf("Usage: cbuild [options] [tasks0 task1...]\n"
               "  Options:\n"
               "    -h, --help, /?          Display this message.\n"
               "    -o, --out-dir           Set global output directory (default=bin).\n"
               "    -c, --configuration     Select project configuration (default=release).\n"
               "    -p, --project           Set project directory/file (default=./).\n");
               return 0;
        }
        else if(argIs("-o") || argIs("--out-dir"))
            global_out_dir = argNext();
        else if(argIs("-c") || argIs("--configuration"))
            configuration = argNext();
        else if(argIs("-p") || argIs("--project"))
            project_dir_or_file = argNext();
        else {
            if(arg[0] == '-')
                throw(cptr_concat("invalid argument: ", arg));
            Autoarr_add(tasks, arg);
        }
    }

    const char* proj_file_path = NULL;
    if(file_exists(project_dir_or_file))
        proj_file_path = project_dir_or_file;
    else if(dir_exists(project_dir_or_file))
        proj_file_path = projectFileFromDir(project_dir_or_file);
    else throw(cptr_concat("can't find a project at path '", project_dir_or_file, "'"));
    
    tryLast(file_open(proj_file_path, FileOpenMode_Read), _m1, ;);
    FileHandle proj_file = _m1.value.VoidPtr;
    char* proj_file_text = NULL;
    tryLast(file_readAll(proj_file, &proj_file_text), _m2, file_close(proj_file));
    file_close(proj_file);

    tryLast(DtsodV24_deserialize(proj_file_text), _m3, free(proj_file_text));
    Hashtable* proj_dtsod = _m3.value.VoidPtr;

    Autoarr_foreach(tasks, task,
        CompilationScenario proj_sc;
        CompilationScenario_construct(&proj_sc);
        tryLast(CompilationScenario_applyProjectOptions(&proj_sc, proj_dtsod, configuration, task), _m4, )        
        tryLast(CompilationScenario_exec(&proj_sc), _m5, ;)
        CompilationScenario_destruct(&proj_sc);
    )
    
#if DEBUG
    Hashtable_free(proj_dtsod);
    free(proj_file_text);
    kt_free();
#endif
    return 0;
}
