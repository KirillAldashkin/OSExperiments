#include "drivers/screen.h"
#include "util.h"

void main() {
	ClearScreen();
	string buffer = "______";
	for (uint16 i = 0; i < 300; i++) {
		ToString(i, 16, buffer);
		WriteLine(buffer);
	}
}