// Позволяет настроить таблицу прерываний.
#ifndef HARDWARE_IDT_H
#define HARDWARE_IDT_H

#include "../utils/types.h"

#define CodeSegment 0x08

// Описание прерывания
#pragma pack (push, 1)
typedef struct {
	uint16 LowOffset;  // Часть адреса функции
	uint16 Segment;    // Сегмент 
	uint8 Reserved;
	uint8 Flags;       // Всякие флаги
	uint16 HighOffset; // Часть адреса функции
} InterruptDescriptor;
#pragma pack (pop)

// Описание таблицы прерываний
#pragma pack (push, 1)
typedef struct {
	uint16 Size;
	uint32 Address;
} InterruptsTable;
#pragma pack (pop)

#define InterruptCount 256
InterruptDescriptor Interrupts[InterruptCount];
InterruptsTable IntTable;

// Устанавливает обработчик в таблице прерываний.
void SetHandler(uint16 index, uint32 address);
// Загружает таблицу прерываний.
void SetInterruptsTable();

#endif