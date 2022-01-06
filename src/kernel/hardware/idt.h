#pragma once

#include <stdint.h>

#define CodeSegment 0x08;


// Описание прерывания
typedef struct {
	uint16_t LowOffset;  // Часть адреса функции
	uint16_t Segment;    // Сегмент 
	uint8_t Reserved;
	uint8_t Flags;       // Всякие флаги
	uint16_t HighOffset; // Часть адреса функции
} __attribute__((packed)) InterruptDescriptor;

// Описание таблицы прерываний
typedef struct {
	uint16_t Size;
	uint32_t Address;
} __attribute__((packed)) InterruptsTable;

#define InterruptCount 256
InterruptDescriptor Interrupts[InterruptCount];
InterruptsTable IntTable;

void SetHandler(uint16_t index, uint32_t address);
void SetInterruptsTable();