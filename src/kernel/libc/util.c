#include "util.h"

void memreverse(char* data, size_t length)
{
	size_t pos = 0;
	while (pos < length) {
		length--;
		char tmp = data[pos];
		data[pos] = data[length];
		data[length] = tmp;
		pos++;
	}
}