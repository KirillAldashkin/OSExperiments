#include "ports.h"

uint8_t PortIn8(uint16_t port) {
    uint8_t result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void PortOut8(uint16_t port, uint8_t val) {
    asm("out %%al, %%dx" : : "a" (val), "d" (port));
}

uint16_t PortIn16(uint16_t port) {
    uint16_t result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void PortOut16(uint16_t port, uint16_t val) {
    asm("out %%ax, %%dx" : : "a" (val), "d" (port));
}