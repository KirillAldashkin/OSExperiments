// Предоставляет функции для работы с портами.
#ifndef HARDWARE_PORTS_H
#define HARDWARE_PORTS_H

#include "../utils/types.h"

// Получает байт из порта.
uint8 PortIn8(uint16 port);
// Передаёт байт в порт.
void PortOut8(uint16 port, uint8 val);

// Получает 2 байта из порта.
uint16 PortIn16(uint16 port);
// Передаёт 2 байта в порт.
void PortOut16(uint16 port, uint16 val);

// Получает 4 байта из порта.
uint32 PortIn32(uint16 port);
// Передаёт 4 байта в порт.
void PortOut32(uint16 port, uint32 val);

// Получает данные из порта по 4 байта.
void PortIn32Buffer(uint16 port, uint32* to, uint32 size);

#endif