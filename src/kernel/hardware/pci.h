// Функции для работы с PCI.
#ifndef HARDWARE_PCI_H
#define HARDWARE_PCI_H

#include "../utils/types.h"

#pragma pack (push, 1)
typedef struct {
	uint16 VendorID;
	uint16 DeviceID;
	uint16 Command;
	uint16 Status;
	uint8 RevisionID;
	uint8 ProgIF;
	uint8 SubClassCode;
	uint8 ClassCode;
	uint8 CacheLineSize;
	uint8 LatencyTimer;
	uint8 HeaderType;
	uint8 BIST;	
} PCIHeaderCommon;
#pragma pack (pop)

#pragma pack (push, 1)
typedef struct {
	PCIHeaderCommon base;
	uint32 BARs[6];
	uint32 CardBusCISptr;
	uint16 SubSystemVendorID;
	uint16 SubSystemID;
	uint32 ExpROMBaseAddress;
	uint8 CapsPtr;
	uint8 Reserved[7];
	uint8 IntLine;
	uint8 IntPin;
	uint8 MinGrant;
	uint8 MaxLatency;
} PCIHeader0;
#pragma pack (pop)

typedef void(*PCIDeviceHandler)(uint8, uint8, uint8, PCIHeaderCommon*);

// Считывает конфигурацию PCI устройства.
uint32 PCIConfigRead(uint8 bus, uint8 slot, uint8 func, uint8 offset);
// Считывает общее начало заголовка PCI устройства.
void PCIGetHeaderCommon(uint8 bus, uint8 slot, uint8 func, PCIHeaderCommon* to);
// Считывает заголовок тип 0 (без общего начала) PCI устройства.
void PCIGetHeader0(uint8 bus, uint8 slot, uint8 func, PCIHeader0* to);
// По заголовку определяет, есть ли PCI устройство.
bool PCIIsValid(PCIHeaderCommon* header);
// Перечисляет все PCI устройства.
void PCIEnumerateDevices(PCIDeviceHandler handler);

#endif