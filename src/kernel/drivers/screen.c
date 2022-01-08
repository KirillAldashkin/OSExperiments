#include "screen.h"
#include "ports.h"
#include "../libc/mem.h"

#define videoMem ((ConsoleChar*)0xb8000)

// Приватное API
uint16_t getCursorOffset() {
	PortOut8(0x3d4, 14);
	uint16_t pos = PortIn8(0x3d5) << 8;
	PortOut8(0x3d4, 15);
	pos += PortIn8(0x3d5);
	return pos;
}
void setCursorOffset(uint16_t offset) {
	PortOut8(0x3d4, 14);
	PortOut8(0x3d5, (uint8_t)(offset >> 8));
	PortOut8(0x3d4, 15);
	PortOut8(0x3d5, (uint8_t)(offset & 0xFF));
}
void scroll() {
	for (uint8_t i = 1; i < MaxRows; i++)
		memcpy(videoMem + MaxCols * (i-1),
			   videoMem + MaxCols * i,
			   MaxCols * 2);
	for (uint16_t i = MaxCols * (MaxRows - 1); i < MaxCols * MaxRows; i++) {
		videoMem[i].text = ' ';
		videoMem[i].attr = AttrBackBlack | AttrTextGray;
	}
}

// Публичное API
void SetCursor(uint8_t x, uint8_t y) { setCursorOffset(x + y * MaxCols); }
Position GetCursor() {
	Position pos;
	uint16_t raw = getCursorOffset();
	pos.x = raw % MaxCols;
	pos.y = raw / MaxCols;
	return pos;
}

void Write(string message) { 
	uint16_t pos = getCursorOffset();
	while (*message != 0) { 
		if (*message == '\n') {
			NewLine();
			pos = getCursorOffset();
			++message;
		} else {
			videoMem[pos++].text = *(message++);
		}
		if (pos >= MaxCols * MaxRows) {
			pos -= MaxCols;
			scroll();
		}
	}
	setCursorOffset(pos);
}
void WriteChar(char c)
{
	uint16_t pos = getCursorOffset();
	videoMem[pos].text = c;
	setCursorOffset(++pos);
}
void WriteLine(string message) {
	Write(message);
	NewLine();
}
void NewLine()
{
	Position pos = GetCursor();
	if (pos.y >= MaxRows - 1) {
		scroll();
		SetCursor(0, MaxRows - 1);
	} else {
		SetCursor(0, pos.y + 1);
	}
}
void Back()
{
	uint16_t pos = getCursorOffset()-1;
	videoMem[pos].text = ' ';
	videoMem[pos].attr = AttrBackBlack | AttrTextGray;
	setCursorOffset(pos);
}
void ClearScreen() {
	for (uint16_t i = 0; i < MaxCols * MaxRows; i++)
	{
		videoMem[i].text = ' ';
		videoMem[i].attr = AttrBackBlack | AttrTextGray;
	}
	setCursorOffset(0);
}