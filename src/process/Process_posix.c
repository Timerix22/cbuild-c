#include "process.h"

#ifndef USE_WINDOWS_PROCESS_API

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
extern int kill (__pid_t __pid, int __sig) __THROW;
extern void * memset(void * __dst, int __val, size_t __n);

#define throw_if_negative(expr) if(expr < 0) throw(cptr_concat(#expr " exited with error ", toString_i64(errno)));
#define safethrow_if_negative(expr) if(expr < 0) safethrow(cptr_concat(#expr " exited with error ", toString_i64(errno)), ;);

Maybe process_start(Process* p, const char* file_path, const char** args, int argc, bool search_in_PATH){
    memset(p, 0, sizeof(Process));
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
        printf("child");
        throw_if_negative(close(input_pipe[1])); // close writing
        throw_if_negative(close(output_pipe[0])); // close reading
        throw_if_negative(close(error_pipe[0])); // close reading

        // redirect io streams to pipes
        throw_if_negative(dup2(input_pipe[0], STDIN_FILENO));
        throw_if_negative(dup2(output_pipe[1], STDOUT_FILENO));
        throw_if_negative(dup2(error_pipe[1], STDERR_FILENO));

        // 
        const char** argv = malloc(sizeof(char*) * (argc+2));
        argv[0] = file_path;
        for(int i=0; i<argc; i++){
            argv[i+1] = args[i];
        }
        argv[argc+1] = NULL;
        // start new process
        int rzlt = search_in_PATH ? execvp(file_path, (char* const*)argv) : execv (file_path, (char* const*)argv);
        exit(rzlt);
    }

    // parent process
    safethrow_if_negative(close(input_pipe[0])); // close reading
    safethrow_if_negative(close(output_pipe[1])); // close writing
    safethrow_if_negative(close(error_pipe[1])); // close writing
    
    p->file_path = file_path;
    p->args = args;
    p->id=pid;
    p->input=input_pipe[1];
    p->output=output_pipe[0];
    p->error=error_pipe[0];
    return MaybeNull;
}

Maybe Process_closeHandles(Process* p){
    safethrow_if_negative(close(p->input));
    safethrow_if_negative(close(p->output));
    safethrow_if_negative(close(p->error));
    p->id=0;
    return MaybeNull;
}

Maybe process_waitForExit(Process* p){
    int wstatus=0;    
    if(waitpid(p->id, &wstatus, 0) == -1)
        safethrow("waitpid() error", ;);
    if(!WIFEXITED(wstatus))
        safethrow(ERR_NOTIMPLEMENTED, ;)
    int exitCode = WEXITSTATUS(wstatus);
    if(exitCode != 0)
        safethrow(cptr_concat("process ", toString_i64(p->id), " exited with code ", toString_i64(exitCode)), ;)
    Process_closeHandles(p);
    return MaybeNull;
}

Maybe Process_stop(Process* p){
    safethrow_if_negative(kill(p->id, SIGINT));
    try(Process_closeHandles(p), _m864, ;);
    return MaybeNull;
}

kt_define(Process, (freeMembers_t)(void*)Process_stop, NULL)

i32 PipeHandle_read(PipeHandle pipe, char* buf, i32 bufsize){
    return read(pipe, buf, bufsize);
}

#endif