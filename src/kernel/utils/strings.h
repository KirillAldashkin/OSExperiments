// Утилиты для работы с символами и строками.
#ifndef UTILS_STRINGS_H
#define UTILS_STRINGS_H

#include "types.h"

// Преобразует данное число в строку.
void SIntToString(int32 number, string str, uint8 radix);
// Преобразует данное число в строку.
void UIntToString(uint32 number, string str, uint8 radix);
// Возвращает длину строки.
uint32 StringLength(string str);
// Сравнивает две строки.
int8 StringCompare(string s1, string s2);

// Проверяет, является ли символ цифрой.
bool CharIsNumber(char c);
// Проверяет, является ли символ буквой.
bool CharIsLetter(char c);
// Проверяет, является ли символ непечатным символом.
bool CharIsSpecial(char c);
// Проверяет, является ли символ прописной буквой.
bool CharIsSmall(char c);
// Преобразует заглавную букву в прописную.
char CharToSmall(char c);
// Проверяет, является ли символ заглавной буквой.
bool CharIsCapital(char c);
// Преобразует прописную букву в заглавную.
char CharToCapital(char c);
// Преобразует все заглавные буквы в строке в прописные.
void StringToSmall(string s);
// Преобразует все прописные буквы в строке в заглавные.
void StringToCapital(string s);
// Преобразует все заглавные буквы в строке в прописные и наоборот.
void StringInverse(string s);

#endif