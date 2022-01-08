#pragma once

#include "types.h"

bool IsNumber(char c);
bool IsLetter(char c);
bool IsSpecial(char c);
bool IsSmall(char c);
char ToSmall(char c);
bool IsCapital(char c);
char ToCapital(char c);
void ToSmallStr(string s);
void ToCapitalStr(string s);
void InverseStr(string s);