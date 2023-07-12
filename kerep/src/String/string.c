#include "string.h"

kt_define(string, NULL, NULL);
Autoarr_define(string, false);

// copies str content to new char pointer value (adding '\0' at the end)
char* string_extract(string str){
    if(str.length==0) return NULL;
    char* cptr=malloc(str.length*sizeof(char)+1);
    cptr[str.length]=0;
    while(str.length-->0)
        cptr[str.length]=str.ptr[str.length];
    return cptr;
}

// copies src.ptr content to new string and adds \0 at the end
string string_copy(string src){
    if(!src.ptr)
        return src;
    string nstr;
    nstr.length=src.length;
    nstr.ptr=malloc(nstr.length+1);
    for(u32 i=0;i<nstr.length;i++)
        nstr.ptr[i]=src.ptr[i];
    nstr.ptr[nstr.length]='\0';
    return nstr;
}

// compares two strings, NullPtr-friendly
bool string_compare(string str0, string str1){
    if(str0.length!=str1.length) return false;
    if(!str0.ptr) return str1.ptr ? false : true;
    else if(!str1.ptr) return false;
    while(str0.length-->0)
        if(*str0.ptr++ != *str1.ptr++) 
            return false;
    return true;
}

// creates new string which is reversed variant of <s>
string string_reverse(string s){
    if(s.length==0) return s;
    string r={malloc(s.length), s.length};
    for(u32 i=0; i<s.length; i++)
        r.ptr[i]=s.ptr[s.length-i-1];
    return r;
}
