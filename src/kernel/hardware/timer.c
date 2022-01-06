#include "isr.h"
#include "timer.h"
#include "../util.h"
#include "../drivers/screen.h"
#include "../drivers/ports.h"

static void TimerCallback(Registers registers);
void InitTimer(uint32 frequency)
{
	// Устанавливаем прерывание
	SetInterruptHandler(IRQ(0), TimerCallback);
	// Базовая частота PIT - 1193180 Гц, вычисляем делитель 
	uint32 div = 1193180 / frequency;
	uint8 low = (uint8)(div & 0xFF);
	uint8 high = (uint8)((div >> 8) & 0xFF);
	// Отправляем комманду
	PortOut8(0x43, 0x36);
	PortOut8(0x40, low);
	PortOut8(0x40, high);
}

uint32 tick = 0;
static void TimerCallback(Registers registers) {
	tick++;
	if (tick > 99999) tick = 0;
	Position pos = GetCursor();
	SetCursor(65, 24);
	string tickStr = "Tick      ";
	ToString(tick, 10, tickStr + 5);
	Write(tickStr);
	SetCursor(pos.x, pos.y);
}