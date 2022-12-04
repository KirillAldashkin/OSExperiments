#include "drives.h"
#include "../drivers/screen.h"
#include "../utils/memory.h"
#include "../utils/strings.h"
#include "parts.h"

#define CACHING_DEBUG false
#define ADDING_DEBUG true

#define FS_MBR_Bootable 0x55AA
#define FS_MBR_Active 0x80
#define FS_MBR_FAT32Mark1 0x0B
#define FS_MBR_FAT32Mark2 0x0C

#define CacheSectorsCount 2016

static uint16 disksRegistered = 0;

#pragma pack (push, 1)
typedef struct {
	uint8 data[512];
	uint32 lba;
	uint16 drive;
	bool dirty;
	bool free;
} SectorCache;

typedef struct {
	uint8 head;
	uint16 sectorCylinder;
} CHS;

typedef struct {
	uint8 activeMark;
	CHS startChs;
	uint8 type;
	CHS end;
	uint32 startLba;
	uint32 sectors;
} MBRPartition;

typedef struct {
	uint8 code[440];
	uint32 signature;
	uint16 reserved;
	MBRPartition partitions[4];
	uint16 bootableMark;
} MBR;
#pragma pack (pop)

#define SectorsCache ((SectorCache*)0x100000)
static uint16 cacheIndex = 0;

uint16 AddDisk(DiskData data) {
	if (disksRegistered == FS_MaxDisks) return FS_TooMuchDisks;
	uint32 size = data.getSize(data.reserved);
#if ADDING_DEBUG
	Write("New drive [");
	WriteU8((uint8)disksRegistered);
	Write("]: ");
	Write(data.getName(data.reserved));
	Write(" (");
	char sizeStr[24];
	uint32 bytes = size / 2 / 1024;
	uint32 szLen = UIntLength(bytes, 10);
	UIntToString(bytes, sizeStr, 10);
	sizeStr[szLen] = '\0';
	Write(sizeStr);
	WriteLine(" MB)");
#endif
	if (size > 0) {
		Disks[disksRegistered] = data;
		disksRegistered++;

		MBR mbr;
		data.read(data.reserved, 0, 1, &mbr);
		for (uint8 part = 0; part < 4; part++) {
			uint8 type = mbr.partitions[part].type;
			if (type == FS_MBR_FAT32Mark1 || type == FS_MBR_FAT32Mark2)
				AddFAT32Partition(disksRegistered-1, mbr.partitions[part].startLba, mbr.partitions[part].sectors);
		}
	}
	return FS_Ok;
}

void InitSectorsCache() {
	for (uint16 i = 0; i < CacheSectorsCount; i++) {
		SectorsCache[i].free = true;
		SectorsCache[i].dirty = false;
	}
}

uint16 ReadSectors(uint16 drive, uint32 lba, uint8 sectors, void* to) {
	if (drive >= disksRegistered) return FS_DriveNotFound;
	uint8* buff = (uint8*)to;

	for (uint8 i = 0; i < sectors; i++) {
		uint32 lbas = lba + i;
#if CACHING_DEBUG
		string trReadMsg = "Reading drive[__] sector[________]";
		MemorySet(trReadMsg + 14, ' ', 2);
		MemorySet(trReadMsg + 25, ' ', 8);
		UIntToString(drive, trReadMsg + 14, 16);
		UIntToString(lbas, trReadMsg + 25, 16);
		WriteLine(trReadMsg);
#endif
		// Ищем запись в кэше
		uint16 inCache = 0xFFFF;
		for (uint16 j = 0; j < CacheSectorsCount; j++) {
			if (SectorsCache[j].free) continue;
			if (SectorsCache[j].lba != lbas) continue;
			inCache = j;
			break;
		}
		if (inCache != 0xFFFF) {
#if CACHING_DEBUG
			string cacheFound = "Found in cache [____]";
			MemorySet(cacheFound + 16, ' ', 4);
			UIntToString(inCache, cacheFound + 16, 16);
			WriteLine(cacheFound);
#endif
			// Есть в кэше, копируем
			MemoryCopy(buff, SectorsCache[inCache].data, SectorBytes);
		} else {
#if CACHING_DEBUG
			string cacheNotFound = "Read to cache [____]";
			MemorySet(cacheNotFound + 15, ' ', 4);
			UIntToString(cacheIndex, cacheNotFound + 15, 16);
			WriteLine(cacheNotFound);
#endif
			// Нету в кэше, считываем
			if (!SectorsCache[cacheIndex].free && SectorsCache[cacheIndex].dirty) {
				WriteLine("!!! Implement cahce writing !!!");
				// TODO Сохраняем эту запись в кэше перед перезаписью
			}
			// Читаем в кэш
			SectorsCache[cacheIndex].free = false;
			SectorsCache[cacheIndex].dirty = false;
			SectorsCache[cacheIndex].lba = lbas;
			uint16 err = Disks[drive].read(Disks[drive].reserved, lbas, 1, SectorsCache[cacheIndex].data);
			if (err != FS_Ok) return err;
			// Есть в кэше, копируем
			MemoryCopy(buff, SectorsCache[cacheIndex].data, SectorBytes);

			cacheIndex = (cacheIndex + 1) % CacheSectorsCount;
		}
		buff += SectorBytes;
	}
	return FS_Ok;
	// TODO считать секторы, учитывая кэш
}

uint16 WriteSectors(uint16 drive, uint32 lba, uint8 sectors, void* to) {
	WriteLine("!!! Implement writing sectors !!!");
	return 0;
}