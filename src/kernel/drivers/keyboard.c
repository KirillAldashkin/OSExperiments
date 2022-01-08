#include "keyboard.h"
#include "ports.h"
#include "../hardware/isr.h"
#include "../libc/stdlib.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../libc/char.h"
#include <stdint.h>

StringHandlerCallback stringHandler = 0;
CharHandlerCallback charHandler = 0;

char buffer[256];
uint16_t pos = 0;
bool caps = 0, shift = 0;

static void KeyboardCallback(Registers* registers);
void InitKeyboard() { 
    SetInterruptHandler(IRQ(1), KeyboardCallback); 
    buffer[255] = '\0';
}
void SetCharHandler(CharHandlerCallback handler) { charHandler = handler; }
void SetStringHandler(StringHandlerCallback handler) { stringHandler = handler; }

#define MaxScanCode 58
const char ASCIIs[] = {
    '\0', '\0', '1', '2', '3', '4', '5',  '6',  '7',  '8', '9',  '0',  '-',  '=',  '\0', '\t',
    'q',  'w',  'e', 'r', 't', 'y', 'u',  'i',  'o',  'p', '[',  ']',  '\n', '\0', 'a',  's', 
    'd',  'f',  'g', 'h', 'j', 'k', 'l',  ';',  '\'', '`', '\0', '\\', 'z',  'x',  'c',  'v',
    'b',  'n',  'm', ',', '.', '/', '\0', '\0', '\0', ' ', '\0'
};

void KeyboardCallback(Registers* registers)
{
	uint8_t scancode = PortIn8(0x60);
    bool down = 1;
    if (scancode >= 0x80) {
        down = 0;
        scancode -= 0x80;
    }
    if (scancode > MaxScanCode) return; // ограничение по количеству кодов
    if ((scancode == 0x2A) || (scancode == 0x36)) { // если Shift
        shift = down;
    } else if ((scancode == 0x0E) && down) { // если Backspace
        if (pos > 0) {
            pos--;
            if (charHandler != 0) charHandler(0);
        }
    } else if ((scancode == 0x3A) && down) { // если CapsLock
        caps = !caps; 
    } else if ((scancode == 0x1C) && down) { // если Enter
        buffer[pos] = '\0';
        if (stringHandler != 0) stringHandler(buffer);
        pos = 0;
    } else if (!down) return; // не отслеживаем отпускания клавиш
    char sym = ASCIIs[scancode];
    if (shift == caps) {
        sym = ToSmall(sym);
    } else {
        sym = ToCapital(sym);
    }
    if (pos == 256 || IsSpecial(sym)) return; // проверка на переполнение и символ
    if (charHandler != 0) charHandler(sym);
    buffer[pos++] = sym;
}