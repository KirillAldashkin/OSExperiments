#pragma once

#include <stdint.h>

uint8_t PortIn8(uint16_t port);
void PortOut8(uint16_t port, uint8_t val);
uint16_t PortIn16(uint16_t port);
void PortOut16(uint16_t port, uint16_t val);