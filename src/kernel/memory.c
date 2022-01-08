#include "memory.h"
#include "libc/types.h"
#include <stdint.h>

// Пока что очень простая система аллокации памяти:
// В таблице по одному биту на каждую страницу - занята или нет.
// При запросе на память ищет в таблице участок нулей нужной длины
// и заполняем этот участок единицами, а возвращаем указатель на
// начало области, которую мы отметили занятой.

#define TableOffset 0x100000 // начало таблицы страниц
#define AllocSize 0x800000 // 8MB
#define PageSize 0x400 // 1KB 
#define PageCount (AllocSize/PageSize) // 8MB/1KB = 8192 страницы
#define TableSize (PageCount/8) // По одному биту на каждую страницу => 1KB
#define AllocOffset (TableOffset+TableSize) // начало страниц

#define ToPage(index) (AllocOffset+(PageSize*(index)))
#define ToIndex(page) (((page)-AllocOffset)/PageSize)
#define ToPages(size) (((size)+PageSize-1)/PageSize)

#define TableIndex(index) ((index)/8)
#define IndexMask(index) (1<<((index)%8))

bool getState(size_t index) {
	uint8_t* bits = (uint8_t*)(TableOffset + TableIndex(index));
	return ((*bits) & IndexMask(index)) > 0;
}
void setState(size_t index, bool state) {
	uint8_t* bits = (uint8_t*)(TableOffset + TableIndex(index));
	if (state) {
		*bits = IndexMask(index) | *bits;
	} else {
		*bits = ~IndexMask(index) & *bits;
	}
}
bool checkAll(int from, size_t length, bool state) {
	if (from < 0 || from + length >= PageCount) return 0;
	for (size_t pos = from; pos < length + from; pos++) {
		if (getState(pos) != state) return 0;
	}
	return 1;
}
void setAll(int from, size_t length, bool state) {
	if (from < 0 || from + length > PageCount) return;
	for (size_t pos = from; pos < length + from; pos++) {
		setState(pos, state);
	}
}
int findFree(size_t length) {
	size_t pos = 0;
	while (pos + length <= PageCount) {
		if (checkAll(pos, length, 0)) return pos;
		pos++;
	}
	return -1;
}

void InitMemory() { setAll(0, PageCount, 0); }
void* AllocateMemory(size_t size) {
	size = ToPages(size + 4); // 4 дополнительных байта хранят выделенный размер
	int index = findFree(size);
	if (index == -1) return (void*)0;
	setAll(index, size, 1);
	*(size_t*)ToPage(index) = size;
	return (void*)(ToPage(index)+4);
}
void FreeMemory(void* start) {
	start = (void*)(((uint8_t*)start) - 4); // смещаем 4 байта (выделенный размер) обратно
	size_t spos = (size_t)start;
	if (spos < AllocOffset || spos >= AllocOffset + AllocSize) return; // проверка на границы
	if ((spos - AllocOffset) % PageSize) return; // проверка на страничность
	int index = ToIndex(spos);
	size_t length = *(size_t*)ToPage(index);
	if (checkAll(index, length, 1)) {
		setAll(index, length, 0);
	}
}