#include "mem.h"

void memcpy(void* to, void* from, size_t length)
{
	size_t index = 0;
	while (index < length) {
		((char*)to)[index] = ((char*)from)[index];
		index++;
	}
}
void* memset(void* to, int value, size_t length)
{
	char* so = (char*)to;
	while (length-- > 0) *(so++) = value;
}
