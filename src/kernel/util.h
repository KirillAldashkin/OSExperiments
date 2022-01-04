#include "types.h"

// Копирует данные из одного участка памяти в другой
void CopyMemory(uint8* from, uint8* to, uint32 length);
// Побайтово переворачивает данные в памяти
void ReverseMemory(uint8* data, uint32 length);
// Записывает данное число в данную строку
void ToString(int32 number, uint8 base, string to);