#pragma once

typedef long int size_t;

// (Вообще, этой функции нету в стандарте...)
// Преобразует данное число в строку
void itoa(int number, char* str, int radix);