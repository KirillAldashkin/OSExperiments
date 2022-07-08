// Упрощает доступ к карте ОЗУ из C.
#ifndef MEMORYMAP_H
#define MEMORYMAP_H

#include "utils/types.h"

#pragma pack (push, 1)
typedef struct {
	uint32 begin;
	uint32 end;
} MemoryMapEntry;
#pragma pack (pop)

#pragma pack (push, 1)
typedef struct {
	uint8 count;
	uint8 reserved[15];
	MemoryMapEntry entries[30];
} MemoryMap;
#pragma pack (pop)

// Указатель на карту ОЗУ.
#define MemMap ((MemoryMap*)0x7FF00)

#endif