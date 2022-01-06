#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../drivers/ports.h"
#include "../libc/stdlib.h"
#include "../types.h"
#include "timer.h"
#include "idt.h"
#include "isr.h"

InterruptHandler handlers[256];

void SetupInterrupts()
{
	// Здесь бы цикл, но указатели на функции всё портят :(
	// ISR-ошибки
	SetHandler(0, (uint32_t)Isr0);
	SetHandler(1, (uint32_t)Isr1);
	SetHandler(2, (uint32_t)Isr2);
	SetHandler(3, (uint32_t)Isr3);
	SetHandler(4, (uint32_t)Isr4);
	SetHandler(5, (uint32_t)Isr5);
	SetHandler(6, (uint32_t)Isr6);
	SetHandler(7, (uint32_t)Isr7);
	SetHandler(8, (uint32_t)Isr8);
	SetHandler(9, (uint32_t)Isr9);
	SetHandler(10, (uint32_t)Isr10);
	SetHandler(11, (uint32_t)Isr11);
	SetHandler(12, (uint32_t)Isr12);
	SetHandler(13, (uint32_t)Isr13);
	SetHandler(14, (uint32_t)Isr14);
	SetHandler(15, (uint32_t)Isr15);
	SetHandler(16, (uint32_t)Isr16);
	SetHandler(17, (uint32_t)Isr17);
	SetHandler(18, (uint32_t)Isr18);
	SetHandler(19, (uint32_t)Isr19);
	SetHandler(20, (uint32_t)Isr20);
	SetHandler(21, (uint32_t)Isr21);
	SetHandler(22, (uint32_t)Isr22);
	SetHandler(23, (uint32_t)Isr23);
	SetHandler(24, (uint32_t)Isr24);
	SetHandler(25, (uint32_t)Isr25);
	SetHandler(26, (uint32_t)Isr26);
	SetHandler(27, (uint32_t)Isr27);
	SetHandler(28, (uint32_t)Isr28);
	SetHandler(29, (uint32_t)Isr29);
	SetHandler(30, (uint32_t)Isr30);
	SetHandler(31, (uint32_t)Isr31);
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
	SetHandler(IRQ(0), (uint32_t)Irq0);
	SetHandler(IRQ(1), (uint32_t)Irq1);
	SetHandler(IRQ(2), (uint32_t)Irq2);
	SetHandler(IRQ(3), (uint32_t)Irq3);
	SetHandler(IRQ(4), (uint32_t)Irq4);
	SetHandler(IRQ(5), (uint32_t)Irq5);
	SetHandler(IRQ(6), (uint32_t)Irq6);
	SetHandler(IRQ(7), (uint32_t)Irq7);
	SetHandler(IRQ(8), (uint32_t)Irq8);
	SetHandler(IRQ(9), (uint32_t)Irq9);
	SetHandler(IRQ(10), (uint32_t)Irq10);
	SetHandler(IRQ(11), (uint32_t)Irq11);
	SetHandler(IRQ(12), (uint32_t)Irq12);
	SetHandler(IRQ(13), (uint32_t)Irq13);
	SetHandler(IRQ(14), (uint32_t)Irq14);
	SetHandler(IRQ(15), (uint32_t)Irq15);

	SetInterruptsTable();
}

void SetupIRQ() {
	asm volatile("sti");
	InitTimer(50);
	InitKeyboard();
}

#define ErrOffset(line) err+38+line*24
void ISRHandler(Registers* registers)
{
	// if(is_not_an_error) {
	//     SomeOtherHandler(registers);
	//     return;
	// }
	string err = "\
|     I'm dead :(     |\n\
|Number     0x00000000|\n\
|Error code 0x00000000|\n\
|EAX        0x00000000|\n\
|EBX        0x00000000|\n\
|ECX        0x00000000|\n\
|EDX        0x00000000|\n\
|ESP        0x00000000|\n\
|EBP        0x00000000|\n\
|ESI        0x00000000|\n\
|EDI        0x00000000|\n\
|DS         0x00000000|\n\
|CS         0x00000000|\n\
|SS         0x00000000|\n\
|EFlags     0x00000000|\n\
|UserEsp    0x00000000|\n\
|EIP        0x00000000|\n";
	itoa(registers->InterruptNumber, ErrOffset(0),  10);
	itoa(registers->ErrorCode,       ErrOffset(1),  10);
	itoa(registers->Eax,             ErrOffset(2),  10);
	itoa(registers->Ebx,             ErrOffset(3),  10);
	itoa(registers->Ecx,             ErrOffset(4),  10);
	itoa(registers->Edx,             ErrOffset(5),  10);
	itoa(registers->Esp,             ErrOffset(6),  10);
	itoa(registers->Ebp,             ErrOffset(7),  10);
	itoa(registers->Esi,             ErrOffset(8),  10);
	itoa(registers->Edi,             ErrOffset(9),  10);
	itoa(registers->Ds,              ErrOffset(10), 10);
	itoa(registers->Cs,              ErrOffset(11), 10);
	itoa(registers->Ss,              ErrOffset(12), 10);
	itoa(registers->EFlags,          ErrOffset(13), 10);
	itoa(registers->UserEsp,         ErrOffset(14), 10);
	itoa(registers->Eip,             ErrOffset(15), 10);
	Write(err);
	asm("cli");
	asm("hlt");
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

void SetInterruptHandler(uint8_t index, InterruptHandler handler) { handlers[index] = handler; }