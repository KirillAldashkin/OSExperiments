#include "idt.h"

void SetHandler(uint16_t index, uint32_t address)
{
	Interrupts[index].LowOffset = (uint16_t)(address & 0xFFFF);
	Interrupts[index].HighOffset = (uint16_t)((address >> 16) & 0xFFFF);
	Interrupts[index].Segment = CodeSegment;
	Interrupts[index].Reserved = 0;
	Interrupts[index].Flags = 0x8E;
}

void SetInterruptsTable()
{
	IntTable.Address = (uint32_t)&Interrupts;
	IntTable.Size = InterruptCount * sizeof(InterruptDescriptor) - 1;
	asm volatile("lidtl (%0)" : : "r" (&IntTable));
}