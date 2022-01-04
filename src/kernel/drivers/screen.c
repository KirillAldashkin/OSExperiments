#include "../types.h"
#include "screen.h"
#include "ports.h"

#define videoMem ((ConsoleChar*)0xb8000)

// Приватное API
uint16 getCursorOffset() {
	PortOut8(0x3d4, 14);
	uint16 pos = PortIn8(0x3d5) << 8;
	PortOut8(0x3d4, 15);
	pos += PortIn8(0x3d5);
	return pos;
}
void setCursorOffset(uint16 offset) {
	PortOut8(0x3d4, 14);
	PortOut8(0x3d5, (uint8)(offset >> 8));
	PortOut8(0x3d4, 15);
	PortOut8(0x3d5, (uint8)(offset & 0xFF));
}

// Публичное API
void SetCursor(uint8 x, uint8 y) { setCursorOffset(x + y * MaxCols); }

void Write(string message) { 
	uint16 pos = getCursorOffset();
	while (*message != 0) videoMem[pos++].text = *(message++);
	setCursorOffset(pos);
}

void WriteLine(string message) {
	uint16 pos = getCursorOffset();
	while (*message != 0) videoMem[pos++].text = *(message++);
	// pos    = y * MaxCols + x => позиция после вывода текста 
	// newPos = (y+1) * MaxCols => берём ближайшее число (>= pos), которое кратно MaxCols
	setCursorOffset((pos-1+MaxCols)/MaxCols*MaxCols);
}

void ClearScreen() {
	for (uint16 i = 0; i < MaxCols * MaxRows; i++)
	{
		videoMem[i].text = ' ';
		videoMem[i].attr = AttrBackBlack | AttrTextGray;
	}
	setCursorOffset(0);
}