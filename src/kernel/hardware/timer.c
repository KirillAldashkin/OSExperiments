#include "isr.h"
#include "timer.h"
#include "../drivers/ports.h"
#include "../libc/stdlib.h"

uint32_t tick = 0;
uint32_t GetTicks() { return tick; }

static void TimerCallback(Registers* registers) { tick++; }
void InitTimer(uint32_t frequency)
{
	// Устанавливаем прерывание
	SetInterruptHandler(IRQ(0), TimerCallback);
	// Базовая частота PIT - 1193180 Гц, вычисляем делитель 
	uint32_t div = 1193180 / frequency;
	uint8_t low = (uint8_t)(div & 0xFF);
	uint8_t high = (uint8_t)((div >> 8) & 0xFF);
	// Отправляем комманду
	PortOut8(0x43, 0x36);
	PortOut8(0x40, low);
	PortOut8(0x40, high);
}