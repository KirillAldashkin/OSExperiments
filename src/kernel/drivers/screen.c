#include "../types.h"
#include "../util.h"
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
void scroll() {
	for (uint8 i = 1; i < MaxRows; i++)
		CopyMemory(videoMem + MaxCols * i,
			       videoMem + MaxCols * (i-1),
			       MaxCols * 2);
	for (uint16 i = MaxCols * (MaxRows - 1); i < MaxCols * MaxRows; i++) {
		videoMem[i].text = ' ';
		videoMem[i].attr = AttrBackBlack | AttrTextGray;
	}
}

// Публичное API
void SetCursor(uint8 x, uint8 y) { setCursorOffset(x + y * MaxCols); }
Position GetCursor() {
	Position pos;
	uint16 raw = getCursorOffset();
	pos.x = raw % MaxCols;
	pos.y = raw / MaxCols;
	return pos;
}
void Write(string message) { 
	uint16 pos = getCursorOffset();
	while (*message != 0) { 
		videoMem[pos++].text = *(message++); 
		if (pos >= MaxCols * MaxRows) {
			pos -= MaxCols;
			scroll();
		}
	}
	setCursorOffset(pos);
}

void WriteLine(string message) {
	Write(message);
	// pos    = y * MaxCols + x => позиция после вывода текста 
	// newPos = (y+1) * MaxCols => берём ближайшее число (>= pos), которое кратно MaxCols
	uint16 pos = getCursorOffset();
	pos = (pos-1+MaxCols)/MaxCols*MaxCols;
	if (pos >= MaxCols*MaxRows) {
		pos -= MaxCols;
		scroll();
	}
	setCursorOffset(pos);
}

void ClearScreen() {
	for (uint16 i = 0; i < MaxCols * MaxRows; i++)
	{
		videoMem[i].text = ' ';
		videoMem[i].attr = AttrBackBlack | AttrTextGray;
	}
	setCursorOffset(0);
}