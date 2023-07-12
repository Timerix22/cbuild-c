#include "DtsodV24.h"
#include "../String/StringBuilder.h"


STRUCT(SerializeSharedData,
    StringBuilder* sh_builder; 
    u8 sh_tabs;
)
#define b shared->sh_builder
#define tabs shared->sh_tabs

Maybe __serialize(StringBuilder* _b, u8 _tabs, Hashtable* dtsod);

#define addc(C) StringBuilder_append_char(b,C)


void __AppendTabs(SerializeSharedData* shared) {
    for (u8 t = 0; t < tabs; t++)
        addc( '\t');
};
#define AppendTabs() __AppendTabs(shared)

Maybe __AppendValue(SerializeSharedData* shared, Unitype u);
#define AppendValue(UNI) __AppendValue(shared, UNI)
Maybe __AppendValue(SerializeSharedData* shared, Unitype u){
    if(u.typeId==ktid_name(i64)){
        StringBuilder_append_i64(b,u.Int64);
    }
    else if(u.typeId==ktid_name(u64)){
        StringBuilder_append_u64(b,u.UInt64);
        addc('u');
    }
    else if(u.typeId==ktid_name(f64)){
        StringBuilder_append_f64(b,u.Float64);
        addc('f');
    }
    else if(u.typeId==ktid_ptrName(char)){
        addc('"');
        char c;
        while((c=*(char*)(u.VoidPtr++))){
            if(c=='\"') addc('\\');
            addc(c);
        }
        addc('"');
    }
    else if(u.typeId==ktid_name(bool)){
        StringBuilder_append_cptr(b, u.Bool ? "true" : "false");
    }
    else if(Unitype_isUniNull(u)){
        safethrow("Null isn't supported in DtsodV24",;);
    }
    else if(u.typeId==ktid_ptrName(Autoarr_Unitype)){
        if(Autoarr_length(((Autoarr_Unitype*)(u.VoidPtr)))){
            addc('\n');
            AppendTabs();
            addc('[');
            tabs++;
            Autoarr_foreach(((Autoarr_Unitype*)(u.VoidPtr)), e, 
                addc('\n');
                AppendTabs();
                try(AppendValue(e),__,;);
                addc(',');
            );
            StringBuilder_rmchar(b);
            addc('\n');
            tabs--;
            AppendTabs();
            addc(']');
        }
        else {
            addc('[');
            addc(']');
        }
    }
    else if(u.typeId==ktid_ptrName(Hashtable)){
        // check hashtable is blank
        bool hashtableNotBlank=false;
        Hashtable_foreach(((Hashtable*)u.VoidPtr), __, 
            hashtableNotBlank=true;
            if(__.key) {} // weird way to disable warning
            break;
        );
        
        if(hashtableNotBlank){
            addc('\n');
            AppendTabs();
            addc('{');
            addc('\n');
            try(__serialize(b,tabs+1,u.VoidPtr),___,;);
            AppendTabs();
            addc('}');
        }
        else {
            addc('{');
            addc('}');
        }
    }
    else {
        dbg((u.typeId)); 
        safethrow(ERR_WRONGTYPE,;);
    }
    return MaybeNull;
};

Maybe __serialize(StringBuilder* _b, u8 _tabs, Hashtable* dtsod){
    SerializeSharedData _shared={
        .sh_builder=_b,
        .sh_tabs=_tabs
    };
    SerializeSharedData* shared=&_shared;

    Hashtable_foreach(dtsod, p, 
        AppendTabs();
        StringBuilder_append_cptr(b,p.key);
        addc(':');
        addc(' ');
        try(AppendValue(p.value),__,;);
        addc(';');
        addc('\n');
    );

    return MaybeNull;
}

Maybe DtsodV24_serialize(Hashtable* dtsod){
    StringBuilder* sb=StringBuilder_create();
    try(__serialize(sb,0,dtsod),__, StringBuilder_free(sb));
    char* str=StringBuilder_build(sb).ptr;
    return SUCCESS(UniHeapPtr(char, str));
}
