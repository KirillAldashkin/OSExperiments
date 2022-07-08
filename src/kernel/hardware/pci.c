#include "pci.h"
#include "ports.h"

#define CommonDoubles sizeof(PCIHeaderCommon)/sizeof(uint32)
#define Header0Doubles sizeof(PCIHeader0)/sizeof(uint32)

uint32 PCIConfigRead(uint8 bus, uint8 slot, uint8 func, uint8 offset) {
    uint32 address = (uint32)(((uint32)bus << 16) | 
							  ((uint32)slot << 11) |
							  ((uint32)func << 8) | 
							  (offset & 0b11111100) | 
							  ((uint32)0x80000000));
 
    PortOut32(0xCF8, address);
    return PortIn32(0xCFC);
}

void PCIGetHeaderCommon(uint8 bus, uint8 slot, uint8 func, PCIHeaderCommon* to) {
	uint32* ptr = (uint32*)to;
	for(uint16 i = 0; i < CommonDoubles; i++)
		*(ptr+i) = PCIConfigRead(bus, slot, func, i*4);	
}

void PCIGetHeader0(uint8 bus, uint8 slot, uint8 func, PCIHeader0* to) {
	uint32* ptr = (uint32*)to;
	if((to->base.HeaderType & 0b01111111) != 0) return;
	for(uint16 i = CommonDoubles; i < Header0Doubles; i++)
		*(ptr+i) = PCIConfigRead(bus, slot, func, i*4);
}

bool PCIIsValid(PCIHeaderCommon* header) {
	return header->VendorID != 0xFFFF;
}

void PCIEnumerateDevices(PCIDeviceHandler handler)
{
	PCIHeaderCommon hdr;
	for (uint16 bus = 0; bus < 256; bus++) {
		for (uint16 slot = 0; slot < 32; slot++) {
			for (uint16 func = 0; func < 8; func++) {
				PCIGetHeaderCommon(bus, slot, func, &hdr);
				if (PCIIsValid(&hdr)) handler(bus, slot, func, &hdr);
			}
		}
	}
}