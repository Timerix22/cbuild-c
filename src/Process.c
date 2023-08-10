#include "Process.h"
#ifdef __MINGW32__
#include <fcntl.h>
#define pipe(fds) _pipe(fds, 1024, _O_BINARY)
int fork(void); // idk where is it's header, different compilers use their own implementations
#endif

void Process_close(Process* p){
    p->id=0;
    close(p->io.input);
    close(p->io.output);
    close(p->io.error);
}

kt_define(Process, (freeMembers_t)Process_close, NULL)

#define throw_if_negative(expr) if(expr == -1) throw(#expr " error");
#define safethrow_if_negative(expr) if(expr == -1) safethrow(#expr " error",;);

Maybe process_start(const char* file_path, const char** args, bool use_PATH){
    int input_pipe[2];
    int output_pipe[2];
    int error_pipe[2];
    safethrow_if_negative(pipe(input_pipe));
    safethrow_if_negative(pipe(output_pipe));
    safethrow_if_negative(pipe(error_pipe));

    int pid = fork();
    if(pid == -1)
        safethrow("fork() error", ;);

    // child process
    if(pid == 0){
        throw_if_negative(close(input_pipe[1])); // close writing
        throw_if_negative(close(output_pipe[0])); // close reading
        throw_if_negative(close(error_pipe[0])); // close reading

        // redirect io streams to pipes
        throw_if_negative(dup2(input_pipe[0], STDIN_FILENO));
        throw_if_negative(dup2(output_pipe[1], STDOUT_FILENO));
        throw_if_negative(dup2(error_pipe[1], STDERR_FILENO));

        // start new process
        int rzlt = use_PATH ? execvp(file_path, (char* const*)args) : execv (file_path, (char* const*)args);
        if(rzlt != 0)

        // end child process
        exit(rzlt);
        return MaybeNull;
    }

    // parent process
    safethrow_if_negative(close(input_pipe[0])); // close reading
    safethrow_if_negative(close(output_pipe[1])); // close writing
    safethrow_if_negative(close(error_pipe[1])); // close writing
    
    Process* p = malloc(sizeof(Process));
    p->file_path = file_path;
    p->args = args;
    p->id=pid;
    p->io.input=input_pipe[1];
    p->io.output=output_pipe[0];
    p->io.error=error_pipe[0];
    return SUCCESS(UniHeapPtr(Process, p));
}
