#include "parts.h"
#include "drives.h"
#include "../drivers/screen.h"
#include "../utils/strings.h"
#include "../utils/memory.h"

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
	uint32 rootDirectoryCluster;
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

typedef struct {
	char name[11];
	uint8 attributes;
	uint8 reservedNT;
	uint8 tenthsSecond;
	uint16 creationTime;
	uint16 creationDate;
	uint16 accessedDate;
	uint16 high16firstCluster;
	uint16 modifiedTime;
	uint16 modifiedDate;
	uint16 low16firstCluster;
	uint32 fileBytes;
} FAT32FileEntry;
#pragma pack (pop)

#define FAT32EntriesPerSector (SectorBytes / sizeof(FAT32FileEntry))
#define FAT32BadCluster 0x0FFFFFF7
#define FAT32Free(cluster) (cluster > 0x0FFFFFF7)
#define FAT32DoNotUse(cluster) (cluster >= 0x0FFFFFF7)

static uint16 partsRegistered = 0;

uint8 readCache[512];

uint32 FAT32nextCluster(uint16 partition, uint32 cluster) {
	uint32 lbaFatStart = Parts[partition].reserved[1];
	uint32 lbaFatOffset = cluster / (SectorBytes / sizeof(uint32));
	uint32 clusterInSector = cluster % (SectorBytes / sizeof(uint32));
	if(PartitionReadSectors(partition, lbaFatStart + lbaFatOffset, 1, readCache) != FS_Ok) return FAT32BadCluster;
	return ((uint32*)readCache)[clusterInSector];
}

uint32 FAT32clusterToSector(uint16 partition, uint32 cluster) {
	uint32 sector = Parts[partition].reserved[0] * (cluster - 2);
	return sector + Parts[partition].reserved[3];
}

bool rootDirGetName(FSEntry* which, string buff, uint16 buffLen) {
	if (which->type != PT_EntryDir) return false;
	if (buffLen < 7) return false;
	MemoryCopy(buff, "pt_____", 7);
	UIntToString(which->partition, buff + 2, 10);
	return true;
}

uint16 index = 0;
bool FAT32moveNext(FSEntryEnumerator* which) {
	index++;
	uint16 partition = which->where->partition;
	//Write("Getting next entry in partition 0x");
	//WriteU16(partition);
	//Write(" | ");
	//WriteU16(index);
	//WriteLine("");

	which->reserved[1]++;
	uint32 sectorsPerCluster = Parts[partition].reserved[0];
	uint32 maxEntryInCluster = FAT32EntriesPerSector * sectorsPerCluster;

	Write("Current cluster 0x");
	WriteU32(which->reserved[0]);
	if (which->reserved[1] > maxEntryInCluster) {
		// двигаемся в следующий кластер
		which->reserved[1] = 0;
		which->reserved[0] = FAT32nextCluster(partition, which->reserved[0]);
		Write(" - next cluster 0x");
		WriteU32(which->reserved[0]);
		if (FAT32DoNotUse(which->reserved[0])) return false;
	}
	WriteLine("");

	uint32 sector = which->reserved[1] / FAT32EntriesPerSector;
	uint32 fSector = FAT32clusterToSector(partition, which->reserved[0]) + sector;
	//Write("Current sector 0x");
	//WriteU32(fSector);
	//WriteLine("");
	if (PartitionReadSectors(partition, fSector, 1, readCache) != FS_Ok) return false;

	uint32 entryIndex = which->reserved[1] % FAT32EntriesPerSector;
	FAT32FileEntry* entry = ((FAT32FileEntry*)readCache) + entryIndex;
	if (entry->attributes == 0) return false;
	which->current.partition= which->where->partition;
	which->current.reserved[0] = (entry->high16firstCluster << 16) + entry->low16firstCluster;
	which->current.reserved[1] = 0;
	which->current.parent = &(which->where->dir);
	char name[12];
	name[11] = '\0';
	MemoryCopy(entry->name, name, 11);
	Write("File size: ");
	WriteU32(entry->fileBytes);
	WriteLine("");
	return true;
}

FSEntryEnumerator FAT32getEnumerator(FSEntry* which) {
	FSEntryEnumerator ret;
	ret.reserved[0] = which->reserved[0]; // текущий кластер
	ret.reserved[1] = 0xFFFFFFFF; // текущая запись
	ret.where = which;
	ret.moveNext = FAT32moveNext;
	return ret;
}

uint16 AddFAT32Partition(uint16 disk, uint32 start, uint32 sectors) {
	if (partsRegistered == PT_MaxParts) return PT_TooMuchParts;

	BPB bpb;
	ReadSectors(disk, start, 1, &bpb);

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

	PartitionData data;
	data.disk = disk;
	data.fsType = PT_PartitonFAT32;
	data.start = start;
	data.sectors = sectors;
	data.root.type = PT_EntryDir;
	data.root.partition = partsRegistered;
	data.root.parent = nullptr;
	data.root.getName = rootDirGetName;
	data.root.reserved[0] = bpb.rootDirectoryCluster; // кластер начала
	data.root.dir.getEnumerator = FAT32getEnumerator;
	data.reserved[0] = bpb.sectorsPerCluster; // секторов на кластер
	data.reserved[1] = bpb.reservedSectors; // LBA начала FAT таблиц
	data.reserved[2] = bpb.sectorsPerFAT32; // секторов на FAT
	data.reserved[3] = bpb.reservedSectors + bpb.fats * bpb.sectorsPerFAT32; // LBA начала данных

	Parts[partsRegistered] = data;
	partsRegistered++;
	return PT_Ok;
}

uint16 PartitionReadSectors(uint16 partition, uint32 start, uint32 sectors, void* to) {
	if (partition >= partsRegistered) return FS_PartNotFound;
	return ReadSectors(Parts[partition].disk, Parts[partition].start + start, sectors, to);
}

uint16 PartitionWriteSectors(uint16 partition, uint32 start, uint32 sectors, void* to) {
	if (partition >= partsRegistered) return FS_PartNotFound;
	return WriteSectors(Parts[partition].disk, Parts[partition].start + start, sectors, to);
}