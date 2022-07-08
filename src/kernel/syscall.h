// Реализация системных вызовов (на будущее). Протокол:
// 1) Поместить в регистр EBX указатель на структуру, описывающую сам запрос
// 2) Вызвать прерывание 0x80
// 3) Ядро обработает указанную структуру, согласно переданному запросу

#ifndef SYSCALL_H
#define SYSCALL_H

#include "hardware/isr.h"
#include "utils/types.h"

#define SysCall(p) asm volatile("int $0x80" :: "b"(&p));

// Обрабатывает системный вызов.
void SystemCall(Registers* registers);

typedef struct {
	uint16 MustBe0;
	uint32 From, To;
} __attribute__((packed)) EchoCall;

#endif