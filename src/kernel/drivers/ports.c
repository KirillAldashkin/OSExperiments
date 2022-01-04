#include "ports.h"

uint8 PortIn8(uint16 port) {
    uint8 result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void PortOut8(uint16 port, uint8 val) {
    __asm__("out %%al, %%dx" : : "a" (val), "d" (port));
}

uint16 PortIn16(uint16 port) {
    uint16 result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void PortOut16(uint16 port, uint16 val) {
    __asm__("out %%ax, %%dx" : : "a" (val), "d" (port));
}