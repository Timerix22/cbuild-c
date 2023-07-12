#include "base.h"
#include "../String/StringBuilder.h"
#include <ctype.h>

// returns length of char buffer (without \0)
u32 cptr_length(const char* str){
    const char *const str_first=str;
    while(*str)
        str++;
    return str-str_first;
}

// allocates new char[] and copies src there
char* cptr_copy(const char* src){
    u32 len=cptr_length(src)+1;
    char* dst=malloc(len);
    while(len--!=0)
        dst[len]=src[len];
    return dst;
}

// multiplies char n times
char* char_multiply(char c, u32 n){
    char* rez=malloc(n+1);
    rez[n]=0;
    while(n--!=0)
        rez[n]=c;
    return rez;
}

bool cptr_equals(const char* key0, const char* key1){
    char c0=*key0;
    char c1=*key1;
    bool eq=c0==c1;
    while(c0 && c1 && eq) {
        c0=*++key0;
        c1=*++key1;
        eq=c0==c1;
    }
    return eq;
}

bool cptr_startsWith(const char* src, const char* fragment){
    char c0=*src;
    char c1=*fragment;
    bool eq=c0==c1 && c0 !=0 && c1!=0;
    while(c0 && c1 && eq) {
        c0=*++src;
        c1=*++fragment;
        eq=c0==c1;
        if(c1==0)
            return true;
    }
    return eq;
}

bool cptr_endsWith(const char* src, const char* fragment){
    u32 src_len=cptr_length(src);
    u32 fr_len=cptr_length(fragment);
    if(src_len<fr_len || src_len==0 || fr_len==0)
        return false;
    src+=src_len-fr_len;
    return cptr_equals(src, fragment);
}


i32 cptr_seek(const char* src, const char* fragment, u32 startIndex, u32 seekLength){
    char sc=*src, fc=*fragment;
    if(sc==0 || fc==0)
        return -1;
    u32 fr_start=startIndex;
    for(u32 si=startIndex; si-startIndex<seekLength && sc!=0; si++){
        sc=src[si];
        fc=fragment[si-fr_start];
        if(fc==0)
            return fr_start;
        if(sc!=fc)
            fr_start++;
    }
    return -1;
}

i32 cptr_seekReverse(const char* src, const char* fragment, u32 startIndex, u32 seekLength){
    char sc=*src, fc=*fragment;
    if(sc==0 || fc==0)
        return -1;
    i32 len=cptr_length(src);
    if(startIndex==(u32)-1)
        startIndex=len-1;
    u32 fr_len=cptr_length(fragment);
    for(u32 si=startIndex; si<(u32)-1 && si!=len-1-seekLength; si--){
        if(si+1<fr_len)
            return -1;
        sc=src[si];
        fc=fragment[0];
        u32 fr_start=si;
        for(u32 fi=0; fc==sc ; fi++){
            if(fi==fr_len)
                return fr_start;
            fc=fragment[fi];
            sc=src[si--];
        }
    }
    return -1;
}

i32 cptr_seekChar(const char* src, char fragment, u32 startIndex, u32 seekLength){
    char sc=*src;
    if(sc==0 || fragment==0)
        return -1;
    for(u32 si=startIndex; si-startIndex<seekLength && sc!=0; si++){
        sc=src[si];
        if(sc==fragment)
            return si;
    }
    return -1;
}

i32 cptr_seekCharReverse(const char* src, char fragment, u32 startIndex, u32 seekLength){
    char sc=*src;
    if(sc==0 || fragment==0)
        return -1;
    i32 len=cptr_length(src);
    if(startIndex==(u32)-1)
        startIndex=len-1;
    for(u32 si=startIndex; si<(u32)-1 && si!=len-1-seekLength; si--){
        sc=src[si];
        if(sc==fragment)
            return si;
    }
    return -1;
}

void memcopy(void* from, void* to, u32 size){
    if(from==NULL || to==NULL)
        throw(ERR_NULLPTR);
    for(u32 i=0; i<size; i++)
        ((char*)to)[i]=((char*)from)[i];
}

char* __cptr_concat(u32 n, ...){
    char** strs=(char**)malloc(n*sizeof(char*));
    u32* lengths=malloc(n*sizeof(u32));
    u32 totalLength=0;

    // reading args from va_list
    va_list vl;
    va_start(vl, n);
    for(u16 i=0; i<n; i++){
        char* str=va_arg(vl,char*);
        i16 length=cptr_length(str);
        strs[i]=str;
        lengths[i]=length;
        totalLength+=length;
    }
    va_end(vl);

    // allocating memory for output value
    char* totality=malloc(totalLength+1);
    char* output=totality;
    totality[totalLength]=0;
    
    // copying content of all strings to rezult
    for(u16 k=0; k<n; k++){
        memcopy(strs[k], totality, lengths[k]);
        totality+=lengths[k];
    }
    
    free(strs);
    free(lengths);
    return output;
}

char* cptr_toLower(const char* src) {
    u32 length=cptr_length(src);
    char *p=malloc(length+1);
    p[length]=0;
    for(u32 i=0; i<length; i++)
        p[i]=tolower(src[i]);
    return p;
}

char* cptr_toUpper(const char* src) {
    u32 length=cptr_length(src);
    char *p=malloc(length+1);
    p[length]=0;
    for(u32 i=0; i<length; i++)
        p[i]=toupper(src[i]);
    return p;
}

char* cptr_replaceCharIn(const char* src, char c_old, char c_new, u32 startIndex, u32 seekLength){
    char* rzlt=cptr_copy(src);
    for(u32 i=startIndex; i!=seekLength && src[i]!=0; i++){
        if(src[i]==c_old)
            rzlt[i]=c_new;
    }
    return rzlt;
}

char* cptr_replaceIn(const char* src, const char* str_old, const char* str_new, u32 startIndex, u32 seekLength){
    StringBuilder* sb=StringBuilder_create();
    const u32 str_old_len=cptr_length(str_old);
    const u32 str_new_len=cptr_length(str_new);
    i32 i=startIndex;
    while( (i=cptr_seek(src, str_old, startIndex, seekLength)) !=-1 ){
        if(i!=0)
            StringBuilder_append_string(sb, (string){.ptr=(char*)src, .length=i});
        StringBuilder_append_string(sb, (string){.ptr=str_new, .length=str_new_len});
        src+=i+str_old_len;
    }
    u32 src_remains_len=cptr_length(src);
    if(src_remains_len>0)
        StringBuilder_append_string(sb, (string){.ptr=(char*)src, .length=src_remains_len});
    string rezult=StringBuilder_build(sb);
    return rezult.ptr;
}
