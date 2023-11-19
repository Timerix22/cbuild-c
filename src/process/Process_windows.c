#include "process.h"

#ifdef USE_WINDOWS_PROCESS_API
#include <windows.h>
#include "../../kerep/src/String/StringBuilder.h"

#define safethrow_if_false(expr) if(!expr) safethrow(cptr_concat(#expr " exited with error ", toString_i64(GetLastError())), ;);

Maybe Process_start(Process* p, const char* file_path, const char** args, int argc, bool search_in_PATH){
    memset(p, 0, sizeof(Process));
    if(search_in_PATH && !cptr_contains(file_path, "\\")){
        LPSTR lpFilePart;
        char search_rezult[MAX_PATH]; 
        safethrow_if_false(SearchPath( NULL, file_path, NULL, MAX_PATH, search_rezult, &lpFilePart))
        file_path = cptr_copy(search_rezult);
    }

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    // Set the bInheritHandle flag so pipe handles are inherited
    saAttr.bInheritHandle = TRUE; 
    saAttr.lpSecurityDescriptor = NULL; 

    HANDLE in_pipe_r = NULL;
    HANDLE in_pipe_w = NULL;
    HANDLE out_pipe_r = NULL;
    HANDLE out_pipe_w = NULL;
    HANDLE err_pipe_r = NULL;
    HANDLE err_pipe_w = NULL;

    // Create a pipe for the child process's STDIN. 
    safethrow_if_false( CreatePipe(&in_pipe_r, &in_pipe_w, &saAttr, 0));
    // Ensure the write handle to the pipe for STDIN is not inherited. 
    safethrow_if_false( SetHandleInformation(in_pipe_w, HANDLE_FLAG_INHERIT, 0) );

    // Create a pipe for the child process's STDOUT. 
    safethrow_if_false( CreatePipe(&out_pipe_r, &out_pipe_w, &saAttr, 0) )
    // Ensure the read handle to the pipe for STDOUT is not inherited.
    safethrow_if_false( SetHandleInformation(out_pipe_r, HANDLE_FLAG_INHERIT, 0) );

    // Create a pipe for the child process's STDERR. 
    safethrow_if_false( CreatePipe(&err_pipe_r, &err_pipe_w, &saAttr, 0) );
    // Ensure the read handle to the pipe for STDERR is not inherited.
    safethrow_if_false( SetHandleInformation(err_pipe_r, HANDLE_FLAG_INHERIT, 0) )

    STARTUPINFO si;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = in_pipe_r;
    si.hStdOutput = out_pipe_w;
    si.hStdError = err_pipe_w;
    si.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi;
    memset(&pi, 0, sizeof(pi));

    StringBuilder* b = StringBuilder_create();
    
    StringBuilder_append_char(b, '"');
    StringBuilder_append_cptr(b, file_path);
    StringBuilder_append_char(b, '"');
    StringBuilder_append_char(b, ' ');
    for(int i = 0; i < argc; i++) {
        StringBuilder_append_char(b, '"');
        StringBuilder_append_cptr(b, args[i]);
        StringBuilder_append_char(b, '"');
        StringBuilder_append_char(b, ' ');
    }
    string args_str = StringBuilder_build(b);

    // Start the child process. 
    if( !CreateProcess(
        file_path,      // Program executable path (optional)
        args_str.ptr,   // Command line args
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        TRUE,           // Inherit IO handles
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)            // Pointer to PROCESS_INFORMATION structure
    ) {
        safethrow(cptr_concat("process_start(", file_path, ", ", args_str, ", ", search_in_PATH ? "true" : "false",
            ") error: CreateProcess() failed with error code ", toString_i64(GetLastError())), ;)
    }
    

    // Close thread handle and child process pipe ends
    safethrow_if_false(CloseHandle(in_pipe_r));
    safethrow_if_false(CloseHandle(out_pipe_w));
    safethrow_if_false(CloseHandle(err_pipe_w));
    safethrow_if_false(CloseHandle(pi.hThread));
    
    p->file_path = file_path;
    p->args = args;
    p->id=pi.dwProcessId;
    p->_winProcHandle = pi.hProcess;

    p->input = in_pipe_w;
    p->output= out_pipe_r;
    p->error = err_pipe_r;
    return MaybeNull;
}

Maybe Process_closeHandles(Process* p){
    safethrow_if_false(CloseHandle(p->_winProcHandle));
    safethrow_if_false(CloseHandle(p->input));
    safethrow_if_false(CloseHandle(p->output));
    safethrow_if_false(CloseHandle(p->error));
    p->id=0;
    return MaybeNull;
}

Maybe Process_waitForExit(Process* p){
    if(WaitForSingleObject(p->_winProcHandle, INFINITE) != 0)
        safethrow("WaitForSingleObject() failed", ;);
    DWORD exitCode = 0;
    safethrow_if_false(GetExitCodeProcess(p->_winProcHandle, &exitCode));
    if(exitCode != 0)
        safethrow(cptr_concat("process ", toString_i64(p->id), " exited with code ", toString_i64(exitCode)), ;)
    Process_closeHandles(p);
    return MaybeNull;
}

Maybe Process_stop(Process* p){
    safethrow_if_false(TerminateProcess(p->_winProcHandle, 1));
    try(Process_closeHandles(p), _m864, ;);
    return MaybeNull;
}

kt_define(Process, (freeMembers_t)(void*)Process_stop, NULL)

i32 PipeHandle_read(PipeHandle pipe, char* buf, i32 bufsize){
    DWORD bytesRead = 0;
    if(!ReadFile(pipe, buf, bufsize, &bytesRead, NULL))
        return -1;
    return bytesRead;
}

#endif