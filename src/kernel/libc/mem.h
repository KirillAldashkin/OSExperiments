#pragma once

#include "stdlib.h"

//  опирует данные из одного участка пам€ти в другой
void memcpy(void* to, void* from, size_t length);
void* memset(void* to, int value, size_t length);