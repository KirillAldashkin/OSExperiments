#include "timer.h"
#include "../hardware/isr.h"
#include "../hardware/ports.h"
#include "../utils/types.h"

volatile uint32 freq, tick = 0;
uint32 GetTicks() { return tick; }
uint32 GetFrequency() { return freq; }

static void timerCallback(Registers* registers) { tick++; }
void InitTimer(uint32 frequency)
{
	freq = frequency;
	// Устанавливаем прерывание
	SetInterruptHandler(IRQ(0), timerCallback);
	// Базовая частота PIT - 1193180 Гц, вычисляем делитель 
	uint32 div = 1193180 / frequency;
	uint8 low = (uint8)(div & 0xFF);
	uint8 high = (uint8)((div >> 8) & 0xFF);
	// Отправляем комманду
	PortOut8(0x43, 0x36);
	PortOut8(0x40, low);
	PortOut8(0x40, high);
}

void Delay(uint32 millis) {
	uint32 to = millis*(freq/1000) + tick;
	while(tick < to) {
		asm volatile("sti");
		asm volatile("hlt");
	}
}