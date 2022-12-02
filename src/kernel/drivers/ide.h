// Драйвер IDE контроллера.
#ifndef DRIVERS_IDE_H
#define DRIVERS_IDE_H

#include "../utils/types.h"

#define SectorBytes 512

#define ATA_Busy              0x80 
#define ATA_DriveReady        0x40 
#define ATA_DriveWriteFault   0x20 
#define ATA_DriveSeekComplete 0x10 
#define ATA_DataRequestReady  0x08 
#define ATA_CorrectedData     0x04 
#define ATA_Index             0x02 
#define ATA_Error             0x01 

#define ATA_BadBlock           0x80
#define ATA_Uncorrectable      0x40
#define ATA_MediaChanged       0x20
#define ATA_MarkNotFound       0x10
#define ATA_MediaChangeRequest 0x08
#define ATA_CommandAborted     0x04
#define ATA_Track0NotFound     0x02
#define ATA_NoAddressMark      0x01

#define ATA_WrongAction            0xFF
#define ATA_WrongDrive             0xFE
#define ATA_NonLBADrive            0xFD
#define ATA_DiskAddressOutOfBounds 0xFC

#define ATA_CommandReadPIO        0x20
#define ATA_CommandReadPIOExt     0x24
#define ATA_CommandReadDMA        0xC8
#define ATA_CommandReadDMAExt     0x25
#define ATA_CommandWritePIO       0x30
#define ATA_CommandWritePIOExt    0x34
#define ATA_CommandWriteDMA       0xCA
#define ATA_CommandWriteDMAExt    0x35
#define ATA_CommandCacheFlush     0xE7
#define ATA_CommandCacheFlushExt  0xEA
#define ATA_CommandPacket         0xA0
#define ATA_CommandIdentifyPacket 0xA1
#define ATA_CommandIdentify       0xEC

#define ATAPI_CommandRead  0xA8
#define ATAPI_CommandEject 0x1B

#define ATA_IdentDeviceType   0
#define ATA_IdentCylinders    2
#define ATA_IdentHeads        6
#define ATA_IdentSectors      12
#define ATA_IdentSerial       20
#define ATA_IdentModel        54
#define ATA_IdentCapabilities 98
#define ATA_IdentFieldValid   106
#define ATA_IdentMaxLBA       120
#define ATA_IdentCommandSets  164
#define ATA_IdentMaxLBAExt    200

#define IDE_ATA   0x00
#define IDE_ATAPI 0x01

#define ATA_Master 0x00
#define ATA_Slave  0x01

#define ATA_RegData        0x00
#define ATA_RegError       0x01
#define ATA_RegFeatures    0x01
#define ATA_RegSecCount0   0x02
#define ATA_RegLBA0        0x03
#define ATA_RegLBA1        0x04
#define ATA_RegLBA2        0x05
#define ATA_RegHDDEVSelect 0x06
#define ATA_RegCommand     0x07
#define ATA_RegStatus      0x07
#define ATA_RegSecCount1   0x08
#define ATA_RegLBA3        0x09
#define ATA_RegLBA4        0x0A
#define ATA_RegLBA5        0x0B
#define ATA_RegControl     0x0C
#define ATA_RegAltStatus   0x0C
#define ATA_RegDevAddress  0x0D

#define ATA_Primary   0x00
#define ATA_Secondary 0x01

#define ATA_Read  0x00
#define ATA_Write 0x01

typedef struct {
	uint16 Base;
	uint16 Control;
	uint16 BusMasterIDE;
	uint8  NoInt;
} IDEChannelRegisters;

typedef struct {
	bool   IsPresent;
	uint8  Channel;
	uint8  Drive;
	bool   IsMaster;
	uint16 IsATAPI;
	uint16 Signature;
	uint16 Capabilities;
	uint32 CommandSets;
	uint32 Sectors;
	char   Model[41];
} IDEDevice;

IDEDevice IDEDevices[4];

// Инициализирует драйвер IDE.
void InitIDE(uint32 bars[5]);
// Считывает данные с IDE контроллера.
uint8 IDERead(uint8 channel, uint8 reg);
// Записывает данные в IDE контроллера.
void IDEWrite(uint8 channel, uint8 reg, uint8 data);
// Считывает массив данных с IDE контроллера.
void IDEReadBuffer(uint8 channel, uint8 reg, void* buffer, uint32 size);
// Ожидает завершение действия в IDE контроллере.
uint8 IDEWait(uint8 channel, bool getStatus);
// Возвращает только ошибку в переданном статусе.
uint8 ParseIDEError(uint8 status);
// Считывает или записывает секторы на диск.
uint8 AccessIDEDrive(uint8 action, uint8 drive, uint32 lba, uint8 sectors, void* data);
#endif