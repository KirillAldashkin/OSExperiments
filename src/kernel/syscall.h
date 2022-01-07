#pragma once

#include "hardware/idt.h"
#include "hardware/isr.h"
#include <stdint.h>

#define SysCall(p) asm volatile("int $0xF0" :: "b"(&p));

// Зарезервируем себе одно прерывание (на будущее) 
// для системных вызовов. Протокол:
// 1) Поместить в регистр EBX указатель на структуру, 
//    описывающую сам запрос
// 2) Вызвать прерывание 0xFF
// 3) Ядро запишет результат в указанную структуру,
//    согласно переданному запросу
void SystemCall(Registers* registers);

typedef struct {
	uint16_t MustBeZero;
	uint32_t From, To;
} __attribute__((packed)) EchoCall;