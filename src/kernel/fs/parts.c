#include "parts.h"
#include "drives.h"
#include "../drivers/screen.h"

#pragma pack (push, 1)
typedef struct {
	// BIOS Parameter Block
	uint8 asmJump[3];
	char oem[8];
	uint16 bytesPerSector;
	uint8 sectorsPerCluster;
	uint16 reservedSectors;
	uint8 fats;
	uint16 rootDirectoryEntries;
	uint16 totalSectors;
	uint8 mediaType;
	uint16 sectorsPerFAT;
	uint16 sectorsPerTrack;
	uint16 heads;
	uint32 hiddenSectors;
	uint32 totalSectorsLarge;
	// Расширенный BIOS Parameter Block для FAT32
	uint32 sectorsPerFAT32;
	uint16 flags;
	uint16 fatVersion;
	uint32 rootDirectoryClusters;
	uint16 fsInfoSector;
	uint16 backupBootSector;
	uint8 reserved[12];
	uint8 driveNumber;
	uint8 NTFlags;
	uint8 aignature;
	uint32 volumeSerial;
	char volumeLabel[11];
	char systemIdentifier[8];
	uint8 bootCode[420];
	uint16 bootableSignature;
} BPB;
#pragma pack (pop)

static uint16 partsRegistered = 0;

uint16 AddFAT32Partition(uint16 disk, uint32 start, uint32 sectors) {
	if (partsRegistered == FS_MaxParts) return PT_TooMuchParts;
	PartitionData data;
	data.disk = disk;
	data.fsType = FS_PartitonFAT32;
	data.start = start;
	data.sectors = sectors;
	BPB bpb;
	Disks[disk].read(Disks[disk].implData, start, 1, &bpb);

	Write("New FAT32 partition [");
	WriteU8((uint8)partsRegistered);
	Write("]: \"");
	char name[12];
	name[11] = '\0';
	MemoryCopy(name, bpb.volumeLabel, 11);
	Write(name);
	Write("\" 0x");
	WriteU32(sectors / 2 / 1024);
	WriteLine("MB");

	parts[partsRegistered] = data;
	partsRegistered++;
	return PT_Ok;
}