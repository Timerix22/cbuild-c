#include "../../kerep/src/base/base.h"

#ifdef _WIN32
#define USE_WINDOWS_PROCESS_API
typedef void* PipeHandle;
#else
typedef int PipeHandle;
#endif

typedef struct Process {
    int id;
    const char* file_path;
    const char** args;
    PipeHandle input;
    PipeHandle output;
    PipeHandle error;
#ifdef USE_WINDOWS_PROCESS_API
    void* _winProcHandle;
#endif
} Process;
kt_declare(Process);

///@param search_in_PATH if true and file_path doesn't contain path separator characters, will search in PATH for the file_path
///@return Maybe<void>
Maybe Process_start(Process* ptr, const char* file_path, const char** args, int argc, bool search_in_PATH);

///@return Maybe<void>
Maybe Process_waitForExit(Process* p);

///@return Maybe<void>
Maybe Process_stop(Process* p);

i32 PipeHandle_read(PipeHandle pipe, char* buf, i32 bufsize);
