#pragma once
#include <stddef.h>

void* AllocateMemory(size_t size);
void FreeMemory(void* start);
void InitMemory();