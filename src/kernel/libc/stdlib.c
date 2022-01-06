#include "stdlib.h"
#include "util.h"

void itoa(int number, char* str, int radix)
{
	if (number == 0) {
		*str = '0';
		return;
	}
	if (number < 0) {
		number *= -1;
		*(str++) = '-';
	}
	size_t len = 0;
	while (number > 0)
	{
		str[len] = number % radix + '0';
		if (str[len] > '9') str[len] += 'A' - '9' - 1;
		number /= radix;
		len++;
	}
	memreverse(str, len);
}