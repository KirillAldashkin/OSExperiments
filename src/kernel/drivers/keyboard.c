#include "keyboard.h"
#include "../hardware/ports.h"
#include "../hardware/isr.h"

CharHandlerCallback charHandler = 0;
volatile uint8 pressCount = 0;

static void keyboardCallback(Registers* registers);

void InitKeyboard() { 
    SetInterruptHandler(IRQ(1), keyboardCallback);
}
void SetCharHandler(CharHandlerCallback handler) { charHandler = handler; }

#define MaxScanCode 58

const char ASCIIs[] = {
    '\0', Escape, '1', '2', '3', '4', '5',    '6',     '7',  '8', '9',    '0',  '-',  '=',      Backspace, '\t',
    'q',  'w',    'e', 'r', 't', 'y', 'u',    'i',     'o',  'p', '[',    ']',  '\n', LControl, 'a',        's', 
    'd',  'f',    'g', 'h', 'j', 'k', 'l',    ';',     '\'', '`', LShift, '\\', 'z',  'x',      'c',        'v',
    'b',  'n',    'm', ',', '.', '/', LShift, PadStar, LAlt, ' ', CapsLock
};

void keyboardCallback(Registers* registers)
{
	uint8 scancode = PortIn8(0x60);

    bool down = true;
    if (scancode >= 0x80) { // если >= 0x80, то клавишу отпустили
        down = false;
        scancode -= 0x80;
    }
    if (scancode > MaxScanCode) return; // ограничение по количеству кодов
	if (down) pressCount++; // для ожидания нажатия клавиши
    if (charHandler != 0) charHandler(down, ASCIIs[scancode]);
}

void WaitKey() {
	volatile uint8 before = pressCount;
	while(before == pressCount) {
		asm volatile("sti");
		asm volatile("hlt");
	}
}