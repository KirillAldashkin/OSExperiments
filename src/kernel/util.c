#include "util.h"

void CopyMemory(uint8* from, uint8* to, uint32 length)
{
	uint32 pos = 0;
	while (pos < length) {
		to[pos] = from[pos];
		pos++;
	}
}

void ReverseMemory(uint8* data, uint32 length)
{
	uint32 pos = 0;
	while (pos < length) {
		length--;
		uint8 tmp = data[pos];
		data[pos] = data[length];
		data[length] = tmp;
		pos++;
	}
}

void ToString(int32 number, uint8 base, string to)
{
	if (number == 0) {
		*to = '0';
		return;
	}
	if (number < 0) {
		number *= -1;
		*(to++) = '-';
	}
	uint8 len = 0;
	while (number > 0)
	{
		to[len] = number % base + '0';
		if (to[len] > '9') to[len] += 'A' - '9' - 1;
		number /= base;
		len++;
	}
	ReverseMemory(to, len);
}