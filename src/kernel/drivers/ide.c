#include "ide.h"
#include "../hardware/ports.h"
#include "screen.h"
#include "timer.h"

IDEChannelRegisters Channels[2];
uint8 IDEBuffer[512] = { 0 };

static volatile bool IDE_Irq = 0;

uint32 implGetSize(uint32* data) {
    IDEDevice* from = (IDEDevice*)data[0];
    uint8 drive = from - IDEDevices;
    if (drive >= MaxDrives) return 0;
    return from->Sectors;
}

string implGetName(uint32* data) {
    IDEDevice* from = (IDEDevice*)data[0];
    uint8 drive = from - IDEDevices;
    if (drive >= MaxDrives) return "<IDE wrong disk pointer>";
    return from->Model;
}

uint16 implRead(uint32* data, uint32 offset, uint8 sectors, void* to) {
    IDEDevice* from = (IDEDevice*)data[0];
    uint8 drive = from - IDEDevices;
    uint8 code = AccessIDEDrive(ATA_Read, drive, offset, sectors, to);
    if (code == 0) return FS_Ok;
    if (code == ATA_WrongDrive) return FS_DriveNotFound;
    if (code == ATA_DiskAddressOutOfBounds) return FS_WrongAddress;
    return FS_DeviceError;
}

uint16 implWrite(uint32* data, uint32 offset, uint8 sectors, void* to) {
    IDEDevice* from = (IDEDevice*)data[0];
    uint8 drive = from - IDEDevices;
    uint8 code = AccessIDEDrive(ATA_Write, drive, offset, sectors, to);
    if (code == 0) return FS_Ok;
    if (code == ATA_WrongDrive) return FS_DriveNotFound;
    if (code == ATA_DiskAddressOutOfBounds) return FS_WrongAddress;
    return FS_DeviceError;
}

void InitIDE(uint32 bars[5]) {
    Channels[ATA_Primary].Base         = (bars[0] & 0xFFFFFFFC) + 0x1F0 * (!bars[0]);
    Channels[ATA_Primary].Control      = (bars[1] & 0xFFFFFFFC) + 0x3F6 * (!bars[1]);
    Channels[ATA_Primary].BusMasterIDE = (bars[4] & 0xFFFFFFFC);

    Channels[ATA_Secondary].Base         = (bars[2] & 0xFFFFFFFC) + 0x170 * (!bars[2]);
    Channels[ATA_Secondary].Control      = (bars[3] & 0xFFFFFFFC) + 0x376 * (!bars[3]);
    Channels[ATA_Secondary].BusMasterIDE = (bars[4] & 0xFFFFFFFC) + 8;

    IDEWrite(ATA_Primary, ATA_RegControl, 2);
    IDEWrite(ATA_Secondary, ATA_RegControl, 2);

    uint8 found = 0;
    for (uint8 channel = 0; channel < 2; channel++) {
        for (uint8 drive = 0; drive < 2; drive++) {
            IDEDevices[found].IsPresent = false;
            uint8 error = 0, isATAPI = false, status;
            
            IDEWrite(channel, ATA_RegHDDEVSelect, 0xA0 | (drive << 4));
            Delay(1);
            IDEWrite(channel, ATA_RegCommand, ATA_CommandIdentify);
            Delay(1);

            if (IDERead(channel, ATA_RegStatus) == 0) continue;

            while (true) {
                status = IDERead(channel, ATA_RegStatus);
                if (status & ATA_Error) { error = 1; break; }
                if (!(status & ATA_Busy) && (status & ATA_DataRequestReady)) break;
            }

            if (error != 0) {
                uint8 low = IDERead(channel, ATA_RegLBA1);
                uint8 high = IDERead(channel, ATA_RegLBA2);

                if ((low == 0x14 && high == 0xEB) || (low == 0x69 && high == 0x96))
                    isATAPI = true;
                else
                    continue; // what is this?!

                IDEWrite(channel, ATA_RegCommand, ATA_CommandIdentifyPacket);
                Delay(1);
            }

            IDEReadBuffer(channel, ATA_RegData, (uint32*)IDEBuffer, 128);

            IDEDevices[found].IsPresent = true;
            IDEDevices[found].IsATAPI = isATAPI;
            IDEDevices[found].Channel = channel;
            IDEDevices[found].Drive = drive;
            uint16* sign = (uint16*)(IDEBuffer + ATA_IdentDeviceType);
            IDEDevices[found].Signature = *sign;
            IDEDevices[found].Capabilities = *((uint16*)(IDEBuffer + ATA_IdentCapabilities));
            IDEDevices[found].CommandSets = *((uint32*)(IDEBuffer + ATA_IdentCommandSets));

            if (IDEDevices[found].CommandSets & (1 << 26))
                IDEDevices[found].Sectors = *((uint32*)(IDEBuffer + ATA_IdentMaxLBAExt));
            else
                IDEDevices[found].Sectors = *((uint32*)(IDEBuffer + ATA_IdentMaxLBA));

            for (uint8 i = 0; i < 40; i += 2) {
                IDEDevices[found].Model[i] = (char)IDEBuffer[ATA_IdentModel + i + 1];
                IDEDevices[found].Model[i + 1] = (char)IDEBuffer[ATA_IdentModel + i];
            }
            IDEDevices[found].Model[40] = '\0';

            DiskData data;
            data.reserved[0] = IDEDevices + found;
            data.getName = (NameGetter)implGetName;
            data.getSize = (SizeGetter)implGetSize;
            data.read = (DataReader)implRead;
            data.write = (DataWriter)implWrite;
            AddDisk(data);

            found++;
        }
    }
}

uint8 IDERead(uint8 channel, uint8 reg) {
    if (reg > 0x07 && reg < 0x0C) 
        IDEWrite(channel, ATA_RegControl, 0x80 | Channels[channel].NoInt);

    uint8 result = 0;
    if (reg < 0x08)
        result = PortIn8(Channels[channel].Base + reg - 0x00);
    else if (reg < 0x0C)
        result = PortIn8(Channels[channel].Base + reg - 0x06);
    else if (reg < 0x0E)
        result = PortIn8(Channels[channel].Control + reg - 0x0A);
    else if (reg < 0x16)
        result = PortIn8(Channels[channel].BusMasterIDE + reg - 0x0E);

    if (reg > 0x07 && reg < 0x0C)
        IDEWrite(channel, ATA_RegControl, Channels[channel].NoInt);
    return result;
}

void IDEWrite(uint8 channel, uint8 reg, uint8 data) {
    if (reg > 0x07 && reg < 0x0C)
        IDEWrite(channel, ATA_RegControl, 0x80 | Channels[channel].NoInt);

    if (reg < 0x08)
        PortOut8(Channels[channel].Base + reg - 0x00, data);
    else if (reg < 0x0C)
        PortOut8(Channels[channel].Base + reg - 0x06, data);
    else if (reg < 0x0E)
        PortOut8(Channels[channel].Control + reg - 0x0A, data);
    else if (reg < 0x16)
        PortOut8(Channels[channel].BusMasterIDE + reg - 0x0E, data);

    if (reg > 0x07 && reg < 0x0C)
        IDEWrite(channel, ATA_RegControl, Channels[channel].NoInt);
}

void IDEReadBuffer(uint8 channel, uint8 reg, void* buffer, uint32 size)
{
    if (reg > 0x07 && reg < 0x0C)
        IDEWrite(channel, ATA_RegControl, 0x80 | Channels[channel].NoInt);

    if (reg < 0x08)
        PortIn32Buffer(Channels[channel].Base + reg - 0x00, buffer, size);
    else if (reg < 0x0C)
        PortIn32Buffer(Channels[channel].Base + reg - 0x06, buffer, size);
    else if (reg < 0x0E)
        PortIn32Buffer(Channels[channel].Control + reg - 0x0A, buffer, size);
    else if (reg < 0x16)
        PortIn32Buffer(Channels[channel].BusMasterIDE + reg - 0x0E, buffer, size);
    
    if (reg > 0x07 && reg < 0x0C)
        IDEWrite(channel, ATA_RegControl, Channels[channel].NoInt);
}

uint8 IDEWait(uint8 channel, bool getStatus) {

    for (int i = 0; i < 4; i++) IDERead(channel, ATA_RegAltStatus);
    while (IDERead(channel, ATA_RegStatus) & ATA_Busy);

    if (!getStatus) return 0;
    return IDERead(channel, ATA_RegStatus);
}

uint8 ParseIDEError(uint8 status) {
    if (status & ATA_Error) return ATA_Error;
    if (status & ATA_DriveWriteFault) return ATA_DriveWriteFault;
    if (!(status & ATA_DataRequestReady)) return ATA_DataRequestReady;
    return 0;
}

uint8 AccessIDEDrive(uint8 action, uint8 drive, uint32 lba, uint8 sectors, void* data) {
    // Проверки
    if (action != ATA_Read && action != ATA_Write) return ATA_WrongAction; // Неверное действие (нужно ATA_Read или ATA_Write)
    if (drive >= MaxDrives) return ATA_WrongDrive;                         // Неверный номер диска
    if (!IDEDevices[drive].IsPresent) return ATA_WrongDrive;               // Нет такого диска
    if (!(IDEDevices[drive].Capabilities & 0x200)) return ATA_NonLBADrive; // Пока поддерживается только диски с LBA адресацией
    if (lba > 0xFFFFFFF) return ATA_DiskAddressOutOfBounds;                // Слишком большой адрес для LBA28

    uint8 channel = IDEDevices[drive].Channel;

    // Отрубаем прерывания
    IDEWrite(channel, ATA_RegControl, Channels[channel].NoInt = (IDE_Irq = 0x0) + 0x02);

    // Готовим данные
    uint8 lbaBuffer[3];
    lbaBuffer[0] =  lba & 0x00000FF;
    lbaBuffer[1] = (lba & 0x000FF00) >> 8;
    lbaBuffer[2] = (lba & 0x0FF0000) >> 16;
    uint8 head = (lba & 0xF000000) >> 24;
    uint16 bus = Channels[channel].Base;
    uint8 cmd;
    if (action == ATA_Read) cmd = ATA_CommandReadPIO;
    if (action == ATA_Write) cmd = ATA_CommandWritePIO;

    // Ждём диск и пишем
    while (IDERead(channel, ATA_RegStatus) & ATA_Busy);

    IDEWrite(channel, ATA_RegHDDEVSelect, 0xE0 | (IDEDevices[drive].Drive << 4) | head);

    IDEWrite(channel, ATA_RegSecCount0, sectors);
    IDEWrite(channel, ATA_RegLBA0, lbaBuffer[0]);
    IDEWrite(channel, ATA_RegLBA1, lbaBuffer[1]);
    IDEWrite(channel, ATA_RegLBA2, lbaBuffer[2]);
    IDEWrite(channel, ATA_RegCommand, cmd);

    uint32* buff = (uint32*)data;
    uint8 error;
    if(action == ATA_Read)
        for (uint8 i = 0; i < sectors; i++) {
            error = ParseIDEError(IDEWait(channel, true));
            if (error) return error;

            PortIn32Buffer(bus, buff, SectorBytes / 4);
            buff += SectorBytes / 4;
        }
    else if(action == ATA_Write)
        for (uint8 i = 0; i < sectors; i++) {
            error = ParseIDEError(IDEWait(channel, true));
            if (error) return error;
            // TODO write to IDE drive
            buff += SectorBytes / 2;
        }
    return 0;
}