#include "base_toString.h"
#include "../base.h"
#include "../../kprint/kprint_format.h"


// accepts char* (ptr to char) and char* (ptr to string)
// uses format kp_s and kp_c to determine what type is <c> argument
char* __toString_char(void* c, u32 fmt) {
    // *c=char*
    if(kp_fmt_dataFormat(fmt)==kp_s){
        return cptr_copy((char*)c); // to avoid segmentation fault on free() when *c allocalet on stack
    }
    // *c=char
    if(kp_fmt_dataFormat(fmt)==kp_c){
        char* cc=malloc(2); 
        cc[0]=*(char*)c;
        cc[1]=0;
        return cc;
    }
    else throw(ERR_FORMAT);
}

char* __toString_bool(void* c, u32 fmt) {
    static const char _strbool[4][6]={ "false", "true\0", "False", "True\0" };
    u8 strind=*(bool*)c==1 + kp_fmt_isUpper(fmt)*2;
    char* rez=malloc(6);
    rez[0]=_strbool[strind][0];
    rez[1]=_strbool[strind][1];
    rez[2]=_strbool[strind][2];
    rez[3]=_strbool[strind][3];
    rez[4]=_strbool[strind][4];
    rez[5]=0;
    return rez;
}

char* toString_i64(i64 n){
    i64 d=n<0 ? -1*n : n;
    char str[32];
    u8 i=sizeof(str);
    str[--i]=0;
    if(d==0)
        str[--i]='0';
    else while(d!=0){
        str[--i]='0' + d%10;
        d/=10;
    }
    if(n<0)
        str[--i]='-';
    return cptr_copy((char*)str+i);
}

char* toString_u64(u64 n, bool withPostfix, bool uppercase){
    char str[32];
    u8 i=sizeof(str);
    str[--i]=0;
    if(withPostfix)
        str[--i]= uppercase ? 'U' : 'u';
    if(n==0)
        str[--i]='0';
    else while(n!=0){
        str[--i]='0' + n%10;
        n/=10;
    }
    return cptr_copy((char*)str+i);
}

#define _toString_float_impl(bufsize, maxPrecision) { \
    char str[bufsize]; \
    if(precision>maxPrecision) \
        throw("too big precision"); \
    if(precision==0) \
        precision=toString_float_default_precision; \
    i32 cn=IFMSC( \
        sprintf_s(str, bufsize, "%.*f", precision, n), \
        sprintf(str, "%.*f", precision, n) \
    ); \
    /* remove trailing zeroes except .0*/ \
    while(str[cn-1]=='0' && str[cn-2]!='.') \
        cn--; \
    if(withPostfix) \
        str[cn++]= uppercase ? 'F' : 'f'; \
    str[cn]='\0'; \
    return cptr_copy(str); \
}

char* toString_f32(f32 n, u8 precision, bool withPostfix, bool uppercase)
    _toString_float_impl(48, toString_f32_max_precision)

char* toString_f64(f64 n, u8 precision, bool withPostfix, bool uppercase)
    _toString_float_impl(512, toString_f64_max_precision)

#define byte_to_bits(byte) { \
    str[cn++]='0' + (u8)((byte>>7)&1); /* 8th bit */ \
    str[cn++]='0' + (u8)((byte>>6)&1); /* 7th bit */ \
    str[cn++]='0' + (u8)((byte>>5)&1); /* 6th bit */ \
    str[cn++]='0' + (u8)((byte>>4)&1); /* 5th bit */ \
    str[cn++]='0' + (u8)((byte>>3)&1); /* 4th bit */ \
    str[cn++]='0' + (u8)((byte>>2)&1); /* 3th bit */ \
    str[cn++]='0' + (u8)((byte>>1)&1); /* 2th bit */ \
    str[cn++]='0' + (u8)((byte>>0)&1); /* 1th bit */ \
}

char* toString_bin(void* _bytes, u32 size, bool inverse, bool withPrefix){
    char* bytes=_bytes;
    char* str=malloc(size*8 + (withPrefix?2:0) +1);
    u32 cn=0; // char number
    if(withPrefix){
        str[cn++]='0';
        str[cn++]='b';
    }
    if(inverse){
        // byte number
        for(i32 bn=size-1; bn>=0; bn--)
            byte_to_bits(bytes[bn])
    } else {
        for(u32 bn=0; bn<size; bn++)
            byte_to_bits(bytes[bn])
    }
    str[cn]=0;
    return str;
}

// converts number from 0 to F to char
char _4bitsHex(u8 u, bool uppercase){
    switch(u){
        case 0: case 1: case 2: case 3: case 4:
        case 5: case 6: case 7: case 8: case 9:
            return '0'+u;
        case 0xA: case 0xB: case 0xC: 
        case 0xD: case 0xE: case 0xF:
            return (uppercase ? 'A' : 'a') + u -10;
        default: 
            dbg(u);
            throw("incorrect number");
            return 219;
    }
}

char* toString_hex(void* _bytes, u32 size, bool inverse, bool withPrefix, bool uppercase){
    char* bytes=_bytes;
    char* str=malloc(size*2 + (withPrefix?2:0) + 1);
    u32 cn=0; // char number
    if(withPrefix){
        str[cn++]='0';
        str[cn++]='x';
    }
    // left to right
    if(inverse){
        // byte number
        for(i32 bn=size-1; bn>=0; bn--){ 
            unsigned char byte=bytes[bn];
            str[cn++]=_4bitsHex(byte/16, uppercase);
            str[cn++]=_4bitsHex(byte%16, uppercase);
        }
    }
    // right to left
    else {
        for(u32 bn=0; bn<size; bn++){ // byte number
            unsigned char byte=bytes[bn];
            str[cn++]=_4bitsHex(byte/16, uppercase);
            str[cn++]=_4bitsHex(byte%16, uppercase);
        }
    }
    str[cn]=0;
    return str;
}


#define __toString_i32_def(BITS) char* __toString_i##BITS(void* _n, u32 f){ \
    switch(kp_fmt_dataFormat(f)){ \
        case kp_i: ; \
            i##BITS n=*(i##BITS*)_n; \
            return toString_i64(n); \
        case kp_b: \
            return toString_bin(_n, BITS/8, getEndian()==LittleEndian, kp_fmt_withPrefix(f)); \
        case kp_h: \
            return toString_hex(_n, BITS/8, getEndian()==LittleEndian, kp_fmt_withPrefix(f), kp_fmt_isUpper(f)); \
        default: \
            kprintf("\n%u\n", kp_fmt_dataFormat(f)); \
            throw(ERR_FORMAT); \
            return NULL; \
    } \
}
__toString_i32_def(8)
__toString_i32_def(16)
__toString_i32_def(32)
__toString_i32_def(64)

#define __toString_u_def(BITS) char* __toString_u##BITS(void* _n, u32 f){ \
    switch(kp_fmt_dataFormat(f)){ \
        case kp_u: ; \
            u##BITS n=*(u##BITS*)_n; \
            return toString_u64(n, kp_fmt_withPostfix(f), kp_fmt_isUpper(f)); \
        case kp_b: \
            return toString_bin(_n, BITS/8, getEndian()==LittleEndian, kp_fmt_withPrefix(f)); \
        case kp_h: \
            return toString_hex(_n, BITS/8, getEndian()==LittleEndian, kp_fmt_withPrefix(f), kp_fmt_isUpper(f)); \
        default: \
            kprintf("\n%u\n", kp_fmt_dataFormat(f)); \
            throw(ERR_FORMAT); \
            return NULL; \
    } \
}
__toString_u_def(8)
__toString_u_def(16)
__toString_u_def(32)
// __toString_u_def(64)
char* __toString_u64(void* _n, u32 f){
    switch(kp_fmt_dataFormat(f)){ 
        case kp_u: ; 
            u64 n=*(u64*)_n; 
            return toString_u64(n, kp_fmt_withPostfix(f), kp_fmt_isUpper(f)); 
        case kp_b: 
            return toString_bin(_n, 64/8, getEndian()==LittleEndian, kp_fmt_withPrefix(f)); 
        case kp_h: 
            return toString_hex(_n, 64/8, getEndian()==LittleEndian, kp_fmt_withPrefix(f), kp_fmt_isUpper(f)); 
        default: 
            kprintf("\n%u\n", kp_fmt_dataFormat(f)); throw(ERR_FORMAT); return NULL; } 
}

#define __toString_float_def(BITS) char* __toString_f##BITS(void* _n, u32 f){ \
    switch(kp_fmt_dataFormat(f)){ \
        case kp_f: ; \
            f##BITS n=*(f##BITS*)_n; \
            return toString_f64(n, toString_float_default_precision, kp_fmt_withPostfix(f), kp_fmt_isUpper(f)); \
        case kp_b: \
            return toString_bin(_n, BITS/8, getEndian()==LittleEndian, kp_fmt_withPrefix(f)); \
        case kp_h: \
            return toString_hex(_n, BITS/8, getEndian()==LittleEndian, kp_fmt_withPrefix(f), kp_fmt_isUpper(f)); \
        default: \
            kprintf("\n%u\n", kp_fmt_dataFormat(f)); \
            throw(ERR_FORMAT); \
            return NULL; \
    } \
}

__toString_float_def(32)
__toString_float_def(64)
