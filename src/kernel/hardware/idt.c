#include "idt.h"

void SetHandler(uint16 index, uint32 address)
{
	Interrupts[index].LowOffset = (uint16)(address & 0xFFFF);
	Interrupts[index].HighOffset = (uint16)((address >> 16) & 0xFFFF);
	Interrupts[index].Segment = CodeSegment;
	Interrupts[index].Reserved = 0;
	Interrupts[index].Flags = 0x8E;
}

void SetInterruptsTable()
{
	IntTable.Address = (uint32)&Interrupts;
	IntTable.Size = InterruptCount * sizeof(InterruptDescriptor) - 1;
	__asm__ __volatile__("lidtl (%0)" : : "r" (&IntTable));
}