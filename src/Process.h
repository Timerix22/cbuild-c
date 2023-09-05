#include "../kerep/src/base/base.h"
#include <unistd.h>

typedef struct {  
    int input;
    int output;
    int error;
} io;

typedef struct {
    int id;
    const char* file_path;
    const char** args;
    io io;
} Process;
kt_declare(Process);

///@return Maybe<Process*>
Maybe process_start(const char* file_path, const char** args, bool use_PATH);

///@return Maybe<void>
Maybe process_waitForExit(Process* p);