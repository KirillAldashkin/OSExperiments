#include "ports.h"

uint8 PortIn8(uint16 port) {
    uint8 result;
    asm volatile("inb %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void PortOut8(uint16 port, uint8 val) {
    asm volatile("outb %%al, %%dx" : : "a" (val), "d" (port));
}

uint16 PortIn16(uint16 port) {
    uint16 result;
    asm volatile("inw %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void PortOut16(uint16 port, uint16 val) {
    asm volatile("outw %%ax, %%dx" : : "a" (val), "d" (port));
}

uint32 PortIn32(uint16 port) {
    uint32 result;
    asm volatile("inl %%dx, %%eax" : "=a" (result) : "d" (port));
    return result;
}

void PortOut32(uint16 port, uint32 val) {
    asm volatile("outl %%eax, %%dx" : : "a" (val), "d" (port));
}

void PortIn32Buffer(uint16 port, uint32* to, uint32 size) {
    asm volatile ("cld; repne; insl;"
                  : "=D" (to), "=c" (size)
                  : "d" (port), "0" (to), "1" (size)
                  : "memory", "cc");
}