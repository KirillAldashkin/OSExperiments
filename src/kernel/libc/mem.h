#pragma once

#include "stdlib.h"
#include <stddef.h>

// Копирует данные из одного участка памяти в другой
void memcpy(void* to, void* from, size_t length);
void* memset(void* to, int value, size_t length);