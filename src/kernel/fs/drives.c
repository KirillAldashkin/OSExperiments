#include "drives.h"
#include "../drivers/screen.h"
#include "../utils/memory.h"
#include "parts.h"

#define FS_MBR_Bootable 0x55AA
#define FS_MBR_Active 0x80
#define FS_MBR_FAT32Mark1 0x0B
#define FS_MBR_FAT32Mark2 0x0C

static uint16 disksRegistered = 0;

#pragma pack (push, 1)
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

uint16 AddDisk(DiskData data) {
	if (disksRegistered == FS_MaxDisks) return FS_TooMuchDisks;
	Write("New drive [");
	WriteU8((uint8)disksRegistered);
	Write("]: ");
	Write(data.getName(data.implData));
	Write(" (0x");
	uint32 size = data.getSize(data.implData);
	WriteU32(size / 2 / 1024);
	WriteLine("MB)");
	if (size > 0) {
		Disks[disksRegistered] = data;

		MBR mbr;
		data.read(data.implData, 0, 1, &mbr);
		for (uint8 part = 0; part < 4; part++) {
			uint8 type = mbr.partitions[part].type;
			if (type == FS_MBR_FAT32Mark1 || type == FS_MBR_FAT32Mark2)
				AddFAT32Partition(disksRegistered, mbr.partitions[part].startLba, mbr.partitions[part].sectors);
		}
		disksRegistered++;
	}
	return FS_Ok;
}