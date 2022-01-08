#pragma once

#define true 1
#define false 0

// (Вообще, этой функции нету в стандарте...)
// Преобразует данное число в строку
void itoa(int number, char* str, int radix);