#include "screen.h"
#include "../hardware/ports.h"
#include "../utils/strings.h"

#define VideoMem ((ConsoleChar*)0xb8000)

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
	MemoryCopy(VideoMem,
		       VideoMem + MaxCols,
		       MaxCols * sizeof(ConsoleChar) * (MaxRows - 1));
	for (uint16 i = MaxCols * (MaxRows - 1); i < MaxCols * MaxRows; i++) {
		VideoMem[i].text = ' ';
		VideoMem[i].attr = AttrBackBlack | AttrTextGray;
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
		if (*message == '\n') {
			NewLine();
			pos = getCursorOffset();
			++message;
		} else {
			VideoMem[pos++].text = *(message++);
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
	uint16 pos = getCursorOffset();
	VideoMem[pos].text = c;
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
	uint16 pos = getCursorOffset()-1;
	VideoMem[pos].text = ' ';
	VideoMem[pos].attr = AttrBackBlack | AttrTextGray;
	setCursorOffset(pos);
}
void ClearScreen() {
	for (uint16 i = 0; i < MaxCols * MaxRows; i++)
	{
		VideoMem[i].text = ' ';
		VideoMem[i].attr = AttrBackBlack | AttrTextGray;
	}
	setCursorOffset(0);
}
void WriteU8(uint8 val) {
	string s = "  ";
	MemorySet(s, '0', 2);
	UIntToString(val, s, 16);
	Write(s);
}
void WriteU16(uint16 val) {
	string s = "    ";
	MemorySet(s, '0', 4);
	UIntToString(val, s, 16);
	Write(s);
}
void WriteU32(uint32 val) {
	string s = "        ";
	MemorySet(s, '0', 8);
	UIntToString(val, s, 16);
	Write(s);
}