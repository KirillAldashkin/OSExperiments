#ifndef FS_DRIVES_H
#define FS_DRIVES_H

#include "../utils/strings.h"
#include "../utils/types.h"

#define SectorBytes 512
#define FS_MaxDisks 16

#define FS_Ok            0
#define FS_DriveNotFound 1
#define FS_WrongAddress  2
#define FS_DeviceError   3
#define FS_TooMuchDisks  4
#define FS_PartNotFound  5

// Функция считывания данных. Принимает: 
// 1) указатель на сведения о диске
// 2) сектор, с которого начать считывание
// 3) количество секторов для считывания
// 4) указатель на буффер для считывания
// Возвращает код ошибки
typedef uint16(*DataReader)(uint32*, uint32, uint8, void*);

// Функция считывания данных. Принимает: 
// 1) указатель на сведения о диске
// 2) сектор, с которого начать запись
// 3) количество секторов для записи
// 4) указатель на данные для записи
// Возвращает код ошибки
typedef uint16(*DataWriter)(uint32*, uint32, uint8, void*);

// Функция возвращения размера. Принимает: 
// 1) указатель на сведения о диске
// Возвращает размер диска в секторах
typedef uint32(*SizeGetter)(uint32*);

// Функция возвращения имени. Принимает: 
// 1) указатель на сведения о диске
// Возвращает имя диска
typedef string(*NameGetter)(uint32*);

#pragma pack (push, 1)
// Сведения о диске
typedef struct {
    DataReader read;
    DataWriter write;
    SizeGetter getSize;
    NameGetter getName;
    uint32 reserved[4];
} DiskData;
#pragma pack (pop)

DiskData Disks[FS_MaxDisks];

// Регистрирует новый диск.
uint16 AddDisk(DiskData data);
// Инициализирует кэш секторов.
void InitSectorsCache();
// Считывает данные с диска.
uint16 ReadSectors(uint16 drive, uint32 lba, uint8 sectors, void* to);
// Записывает данные на диск.
uint16 WriteSectors(uint16 drive, uint32 lba, uint8 sectors, void* to);
#endif
