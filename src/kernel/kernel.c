#include "hardware/isr.h"
#include "hardware/timer.h"
#include "drivers/screen.h"
#include "drivers/keyboard.h"

void main() {
    SetupInterrupts();
    asm volatile("sti");
    ClearScreen();
    InitKeyboard();
}