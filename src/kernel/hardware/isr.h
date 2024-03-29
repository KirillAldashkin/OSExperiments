// Определяет обработчики прерываний и позволяет задать свои.
#ifndef HARDWARE_ISR_H
#define HARDWARE_ISR_H

#include "../utils/types.h"

// Обработчики ошибок процессора
extern void Isr0();
extern void Isr1();
extern void Isr2();
extern void Isr3();
extern void Isr4();
extern void Isr5();
extern void Isr6();
extern void Isr7();
extern void Isr8();
extern void Isr9();
extern void Isr10();
extern void Isr11();
extern void Isr12();
extern void Isr13();
extern void Isr14();
extern void Isr15();
extern void Isr16();
extern void Isr17();
extern void Isr18();
extern void Isr19();
extern void Isr20();
extern void Isr21();
extern void Isr22();
extern void Isr23();
extern void Isr24();
extern void Isr25();
extern void Isr26();
extern void Isr27();
extern void Isr28();
extern void Isr29();
extern void Isr30();
extern void Isr31();

// IRQ прерывания
extern void Irq0();
extern void Irq1();
extern void Irq2();
extern void Irq3();
extern void Irq4();
extern void Irq5();
extern void Irq6();
extern void Irq7();
extern void Irq8();
extern void Irq9();
extern void Irq10();
extern void Irq11();
extern void Irq12();
extern void Irq13();
extern void Irq14();
extern void Irq15();

// Системный вызов
// extern void SysCall();

#define IRQ(n) (n+32)

// Структура со всякими регистрами (и кодом ошибки)
typedef struct {
	uint32 Ds;
	uint32 Edi, Esi, Ebp, Esp, Ebx, Edx, Ecx, Eax;
	uint32 InterruptNumber, ErrorCode;
	uint32 Eip, Cs, EFlags, UserEsp, Ss;
} Registers;
typedef void(*InterruptHandler)(Registers*);

// Настраивает прерывания.
void SetupInterrupts();
// Инициализирует IRQ.
void SetupIRQ();
// Задаёт обработчик прерываний.
void SetInterruptHandler(uint8 index, InterruptHandler handler);

#endif