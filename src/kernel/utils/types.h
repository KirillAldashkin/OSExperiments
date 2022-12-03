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

#endif