#include "string.h"

size_t strlen(char* str) {
	size_t len = 0;
	while (str[len] != 0) ++len;
	return len;
}

int strcmp(char* s1, char* s2)
{
	while (1) {
		if (*s1 == 0 && *s2 == 0) return 0;
		if (*s1 != *s2) return *s1 - *s2;
		s1++;
		s2++;
	}
}
