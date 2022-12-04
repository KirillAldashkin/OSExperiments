#include "strings.h"
#include "memory.h"

void SIntToString(int32 number, string str, uint8 radix)
{
	if (number == 0) {
		*str = '0';
		return;
	}
	if (number < 0) {
		number *= -1;
		*(str++) = '-';
	}
	uint32 len = 0;
	while (number > 0)
	{
		str[len] = number % radix + '0';
		if (str[len] > '9') str[len] += 'A' - '9' - 1;
		number /= radix;
		len++;
	}
	MemoryReverse(str, len);
}

void UIntToString(uint32 number, string str, uint8 radix)
{
	if (number == 0) {
		*str = '0';
		return;
	}
	uint32 len = 0;
	while (number > 0)
	{
		str[len] = number % radix + '0';
		if (str[len] > '9') str[len] += 'A' - '9' - 1;
		number /= radix;
		len++;
	}
	MemoryReverse(str, len);
}

uint32 StringLength(string str) {
	uint32 len = 0;
	while (str[len] != 0) ++len;
	return len;
}

int8 StringCompare(string s1, string s2)
{
	while (true) {
		if (*s1 == 0 && *s2 == 0) return 0;
		if (*s1 < *s2) return -1;
		if (*s1 > *s2) return 1;
		s1++;
		s2++;
	}
}

bool StartsWith(string str, string prefix) {
	uint32 len = StringLength(prefix);
	for (uint32 i = 0; i < len; i++) {
		if (str[i] != prefix[i]) return false;
	}
	return true;
}

uint32 SIntLength(int32 number, uint8 radix) {
	if (number == MIN_S32) return 11;
	if (number == 0) return 1;

	uint32 len = 0;
	if (number < 0) {
		len++;
		number *= -1;
	}

	while (number > 0) {
		number /= radix;
		len++;
	}
	return len;
}

uint32 UIntLength(uint32 number, uint8 radix) {
	if (number == 0) return 1;

	uint32 len = 0;

	while (number > 0) {
		number /= radix;
		len++;
	}
	return len;
}

bool CharIsNumber(char c) { return c >= '0' || c <= '9'; }
bool CharIsLetter(char c) { return CharIsSmall(c) || CharIsCapital(c); }
bool CharIsSmall(char c) { return c >= 'a' && c <= 'z'; }
bool CharIsCapital(char c) { return c >= 'A' && c <= 'Z'; }
bool CharIsSpecial(char c) { return c < ' '; }

char CharToSmall(char c) {
	if (CharIsCapital(c)) c -= 'A' - 'a';
	return c;
}

char CharToCapital(char c) {
	if (CharIsSmall(c)) c += 'A' - 'a';
	return c;
}

void StringToSmall(string s) {
	for (; *s != 0; ++s) {
		*s = CharToSmall(*s);
	}
}

void StringToCapital(string s) {
	for (; *s != 0; ++s) {
		*s = CharToCapital(*s);
	}
}

void StringInverse(string s){
	for (; *s != 0; ++s) {
		if (CharIsCapital(*s)) {
			*s = CharToSmall(*s);
		} else {
			*s = CharToCapital(*s);
		}
	}
}