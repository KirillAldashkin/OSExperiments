#include "memory.h"

void MemoryCopy(void* to, void* from, uint32 length)
{
	uint32 index = 0;
	while (index < length) {
		((int8*)to)[index] = ((int8*)from)[index];
		index++;
	}
}

void MemoryReverse(void* data, uint32 length)
{
	uint8* ptr = (uint8*)data;
	uint32 pos = 0;
	while (pos < length) {
		length--;
		char tmp = ptr[pos];
		ptr[pos] = ptr[length];
		ptr[length] = tmp;
		pos++;
	}
}

void MemorySet(void* to, uint8 value, uint32 length)
{
	uint8* so = (uint8*)to;
	while (length-- > 0) *(so++) = value;
}