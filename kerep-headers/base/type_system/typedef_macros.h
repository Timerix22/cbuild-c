#pragma once

#define ENUM(ENUM_NAME, ENUM_MEMBERS...) typedef enum ENUM_NAME { \
    ENUM_MEMBERS \
} ENUM_NAME;

#define PACKED_ENUM(ENUM_NAME, ENUM_MEMBERS...) typedef enum ENUM_NAME { \
    ENUM_MEMBERS \
} __attribute__((__packed__)) ENUM_NAME;

#define STRUCT(STRUCT_NAME, STRUCT_MEMBERS...) typedef struct STRUCT_NAME STRUCT_NAME; \
typedef struct STRUCT_NAME { \
    STRUCT_MEMBERS \
} STRUCT_NAME; \
kt_declare(STRUCT_NAME);
