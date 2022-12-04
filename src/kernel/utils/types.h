// Определяет базовые типы.
#ifndef UTILS_TYPES_H
#define UTILS_TYPES_H

#define true 1
#define false 0
#define nullptr ((void*)0)

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

typedef unsigned char bool;
typedef char* string;

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int int32;
typedef short int16;
typedef char int8;

#define MIN_U8 0
#define MAX_U8 255
#define MIN_S8 -128
#define MAX_S8 127

#define MIN_U16 0
#define MAX_U16 65536
#define MIN_S16 -32768
#define MAX_S16 32767

#define MIN_U32 0
#define MAX_U32 4294967295
#define MIN_S32 -2147483648
#define MAX_S32 2147483647

#endif