#include "keyboard.h"
#include "screen.h"
#include "ports.h"
#include "../hardware/isr.h"
#include "../util.h"

static void KeyboardCallback(Registers registers);
void InitKeyboard() { SetInterruptHandler(IRQ(1), KeyboardCallback); }

void GetChar(uint8 scan, string to);
void KeyboardCallback(Registers registers)
{
	uint8 scancode = PortIn8(0x60);
	string scanstr = "Scancode: 0x  ";
	ToString(scancode, 16, scanstr + 12);
	WriteLine(scanstr);
	string charstr = "                            ";
	CopyMemory("Symbol:                     ", charstr, 28);
	GetChar(scancode, charstr + 8);
	WriteLine(charstr);
}

string codes[] = {
    /* 0x0# */ "Error", "Escape", "1", "2", "3", "4", "5",           "6",        "7",        "8",      "9",          "0",  "-",     "+",         "Backspace", "Tab",
	/* 0x1# */ "Q",     "W",      "E", "R", "T", "Y", "U",           "I",        "O",        "P",      "[",          "]",  "Enter", "Left Ctrl", "A",         "S",
	/* 0x2# */ "D",     "F",      "G", "H", "J", "K", "L",           ";",        "'",        "`",      "Left Shift", "\\", "Z",     "X",         "C",         "V",
	/* 0x3# */ "B",     "N",      "M", ",", ".", "/", "Right Shift", "* Keypad", "Left Alt", "Space"
};
void GetChar(uint8 scan, string to) {
	if (scan >= 0x80) { 
		CopyMemory("[Up]", to, 4);
		to += 4;
		scan -= 0x80; 
	}
	if (scan <= 0x39) {
		CopyMemory(codes[scan], to, Length(codes[scan]));
	}
}