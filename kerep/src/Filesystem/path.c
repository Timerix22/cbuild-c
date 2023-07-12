#include "filesystem.h"

char* __path_concat(u32 n, ...){
    char** parts=(char**)malloc(n*sizeof(char*));
    u32* lengths=malloc(n*sizeof(u32));
    u32 totalLength=0;

    // reading args from va_list
    va_list vl;
    va_start(vl, n);
    for(u16 i=0; i<n; i++){
        char* part=va_arg(vl,char*);
        i16 length=cptr_length(part);
        parts[i]=part;
        lengths[i]=length;
        totalLength+=length;
    }
    va_end(vl);

    // allocating memory for output value
    char* totality=malloc(totalLength+1);
    const char* output=totality;
    totality[totalLength]=0;
    
    // copying content of all strings to rezult
    u16 k=0;
    for(; k<n-1; k++){
        memcopy(parts[k], totality, lengths[k]);
        totality+=lengths[k];
        *totality=path_sep;
        totality++;
    }
    memcopy(parts[k], totality, lengths[k]);

    free(parts);
    free(lengths);
    return output;
}

char* path_fixSeparators(const char* path){
    char* pathCopy=cptr_copy(path);
    char c;
    while((c=*pathCopy)){
        if(c==path_notSep)
            *pathCopy=path_sep;
        pathCopy++;
    }
    return pathCopy;
}

Maybe path_throwIfEscapes(const char* path){
    if(cptr_contains(path,".."))
        safethrow(cptr_concat("path <",path,"> uses <..>, that's not allowed"),);
    return MaybeNull;
}

char* path_parentDir(char* dir){
    char* copy=cptr_copy(dir);
    i32 length=cptr_length(copy);
    i32 i=cptr_lastIndexOfChar(copy,path_sep);
    if(i!=-1 && i==length-1){
        copy[length-1]=0;
        i=cptr_lastIndexOfChar(copy,path_sep);
    }
    if(i==-1){
        free(copy);
        copy=malloc(2);
        copy[0]='.';
        copy[1]=0;
    }
    return copy;
}


char* path_basename(char* path, bool with_extension){
    i32 nameIndex=cptr_lastIndexOfChar(path, path_sep)+1;
    string rezult=string_fromCptr(path+nameIndex);
    if(!with_extension){
        i32 extIndex=cptr_lastIndexOfChar(rezult.ptr, '.');
        if(extIndex!=0 && extIndex!=-1)
            rezult.length=extIndex;
    }
    return string_extract(rezult);
}
