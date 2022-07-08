#include "shell.h"
#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "utils/strings.h"
#include "utils/memory.h"

#define buffer ((char*)0x7FD00) // Буфер для входящих клавиш
#define bufLen 255

bool shift = false, caps = false;
bool listen = true;
uint16 pos = 0;
InputHandler handler;

void shellHandleChar(bool down, char sym);

void ShellInit(InputHandler nhandler) {
	MemorySet(buffer, 0, bufLen+1);
	handler = nhandler;
	SetCharHandler(shellHandleChar);
}

void shellHandleChar(bool down, char sym) {
	if(!listen) return;
	if(sym == LShift || sym == RShift) {
		shift = down; // Shift работает, пока нажат
	} else if(sym == CapsLock && down) {
		caps = !caps; // Caps переключается в момент нажатия
	} else if(sym == '\n' && down) {
		// При нажатии Enter передаём строку обработчику и очищаем буфер
		buffer[pos] = '\0';
		pos = 0;
		NewLine();
		listen = false;
		handler(buffer);
		listen = true;
	} else if(sym == Backspace && down && pos > 0) {
		// При Backspace убираем клавишу с экрана и из буфера
		buffer[pos--] = '\0';
		Back();
	} else if(sym > 0 && down && pos < bufLen) {
		// Иначе просто выводим
		buffer[pos++] = sym;
		if(shift != caps) sym = CharToCapital(sym);
		WriteChar(sym);
	}
}