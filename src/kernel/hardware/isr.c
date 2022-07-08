#include "isr.h"
#include "idt.h"
#include "../drivers/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../utils/strings.h"
#include "../utils/memory.h"
#include "ports.h"

InterruptHandler handlers[256];

void SetupInterrupts()
{
	// Здесь бы цикл, но указатели на функции всё портят :(
	// ISR-ошибки
	SetHandler(0, (uint32)Isr0);
	SetHandler(1, (uint32)Isr1);
	SetHandler(2, (uint32)Isr2);
	SetHandler(3, (uint32)Isr3);
	SetHandler(4, (uint32)Isr4);
	SetHandler(5, (uint32)Isr5);
	SetHandler(6, (uint32)Isr6);
	SetHandler(7, (uint32)Isr7);
	SetHandler(8, (uint32)Isr8);
	SetHandler(9, (uint32)Isr9);
	SetHandler(10, (uint32)Isr10);
	SetHandler(11, (uint32)Isr11);
	SetHandler(12, (uint32)Isr12);
	SetHandler(13, (uint32)Isr13);
	SetHandler(14, (uint32)Isr14);
	SetHandler(15, (uint32)Isr15);
	SetHandler(16, (uint32)Isr16);
	SetHandler(17, (uint32)Isr17);
	SetHandler(18, (uint32)Isr18);
	SetHandler(19, (uint32)Isr19);
	SetHandler(20, (uint32)Isr20);
	SetHandler(21, (uint32)Isr21);
	SetHandler(22, (uint32)Isr22);
	SetHandler(23, (uint32)Isr23);
	SetHandler(24, (uint32)Isr24);
	SetHandler(25, (uint32)Isr25);
	SetHandler(26, (uint32)Isr26);
	SetHandler(27, (uint32)Isr27);
	SetHandler(28, (uint32)Isr28);
	SetHandler(29, (uint32)Isr29);
	SetHandler(30, (uint32)Isr30);
	SetHandler(31, (uint32)Isr31);
	// Общаемся с PIC'ом
	PortOut8(0x20, 0x11);
	PortOut8(0xA0, 0x11);
	PortOut8(0x21, 0x20);
	PortOut8(0xA1, 0x28);
	PortOut8(0x21, 0x04);
	PortOut8(0xA1, 0x02);
	PortOut8(0x21, 0x01);
	PortOut8(0xA1, 0x01);
	PortOut8(0x21, 0x0);
	PortOut8(0xA1, 0x0);
	// IRQ-прерывания
	SetHandler(IRQ(0), (uint32)Irq0);
	SetHandler(IRQ(1), (uint32)Irq1);
	SetHandler(IRQ(2), (uint32)Irq2);
	SetHandler(IRQ(3), (uint32)Irq3);
	SetHandler(IRQ(4), (uint32)Irq4);
	SetHandler(IRQ(5), (uint32)Irq5);
	SetHandler(IRQ(6), (uint32)Irq6);
	SetHandler(IRQ(7), (uint32)Irq7);
	SetHandler(IRQ(8), (uint32)Irq8);
	SetHandler(IRQ(9), (uint32)Irq9);
	SetHandler(IRQ(10), (uint32)Irq10);
	SetHandler(IRQ(11), (uint32)Irq11);
	SetHandler(IRQ(12), (uint32)Irq12);
	SetHandler(IRQ(13), (uint32)Irq13);
	SetHandler(IRQ(14), (uint32)Irq14);
	SetHandler(IRQ(15), (uint32)Irq15);
	// Системный вызов
	SetHandler(0x80, (uint32)SysCall);

	SetInterruptsTable();
}

void SetupIRQ() {
	asm volatile("sti");
	InitTimer(10000);
	InitKeyboard();
}

#define ErrOffset(line) err+38+line*24
void ISRHandler(Registers* registers)
{
	string err = "\
|     I'm dead :(     |\n\
|Number     0x        |\n\
|Error code 0x        |\n\
|EAX        0x        |\n\
|EBX        0x        |\n\
|ECX        0x        |\n\
|EDX        0x        |\n\
|ESP        0x        |\n\
|EBP        0x        |\n\
|ESI        0x        |\n\
|EDI        0x        |\n\
|DS         0x        |\n\
|CS         0x        |\n\
|SS         0x        |\n\
|EFlags     0x        |\n\
|UserEsp    0x        |\n\
|EIP        0x        |\n";
	UIntToString(registers->InterruptNumber, ErrOffset(0),  16);
	UIntToString(registers->ErrorCode,       ErrOffset(1),  16);
	UIntToString(registers->Eax,             ErrOffset(2),  16);
	UIntToString(registers->Ebx,             ErrOffset(3),  16);
	UIntToString(registers->Ecx,             ErrOffset(4),  16);
	UIntToString(registers->Edx,             ErrOffset(5),  16);
	UIntToString(registers->Esp,             ErrOffset(6),  16);
	UIntToString(registers->Ebp,             ErrOffset(7),  16);
	UIntToString(registers->Esi,             ErrOffset(8),  16);
	UIntToString(registers->Edi,             ErrOffset(9),  16);
	UIntToString(registers->Ds,              ErrOffset(10), 16);
	UIntToString(registers->Cs,              ErrOffset(11), 16);
	UIntToString(registers->Ss,              ErrOffset(12), 16);
	UIntToString(registers->EFlags,          ErrOffset(13), 16);
	UIntToString(registers->UserEsp,         ErrOffset(14), 16);
	UIntToString(registers->Eip,             ErrOffset(15), 16);
	Write(err);
	asm volatile("cli");
	asm volatile("hlt");
}

void IRQHandler(Registers* registers)
{
	// Говорим PIC'у: "Понял, принял, обработал."
	if (registers->InterruptNumber >= IRQ(8)) PortOut8(0xA0, 0x20); // slave PIC
	PortOut8(0x20, 0x20); // master PIC
	// Вызываем прерывание
	if (handlers[registers->InterruptNumber] != 0) {
		handlers[registers->InterruptNumber](registers);
	}
}

void SetInterruptHandler(uint8 index, InterruptHandler handler) { handlers[index] = handler; }