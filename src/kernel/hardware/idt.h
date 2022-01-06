#pragma once

#include "../types.h"

#define CodeSegment 0x08;


// Описание прерывания
typedef struct {
	uint16 LowOffset;  // Часть адреса функции
	uint16 Segment;    // Сегмент 
	uint8 Reserved;
	uint8 Flags;       // Всякие флаги
	uint16 HighOffset; // Часть адреса функции
} __attribute__((packed)) InterruptDescriptor;

// Описание таблицы прерываний
typedef struct {
	uint16 Size;
	uint32 Address;
} __attribute__((packed)) InterruptsTable;

#define InterruptCount 256
InterruptDescriptor Interrupts[InterruptCount];
InterruptsTable IntTable;

void SetHandler(uint16 index, uint32 address);
void SetInterruptsTable();