// Утилиты для работы с памятью.
#ifndef UTILS_MEMORY_H
#define UTILS_MEMORY_H

#include "types.h"

// Копирует данные из одного участка памяти в другой.
void MemoryCopy(void* to, void* from, uint32 length);
// Побайтово переворачивает данные в памяти.
void MemoryReverse(void* data, uint32 length);
// Заполняет участок памяти.
void MemorySet(void* to, uint8 value, uint32 length);

#endif