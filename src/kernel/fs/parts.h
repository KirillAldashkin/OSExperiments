#ifndef FS_PARTS_H
#define FS_PARTS_H

#include "../utils/types.h"

#define FS_MaxParts 128

#define PT_Ok            0
#define PT_TooMuchParts  1

#pragma pack (push, 1)
// Сведения о разделе
typedef struct {
    uint16 disk;
    uint16 fsType;
    uint32 start;
    uint32 sectors;
    void* data;
} PartitionData;
#pragma pack (pop)

PartitionData parts[FS_MaxParts];

uint16 AddFAT32Partition(uint16 disk, uint32 start, uint32 sectors);
#endif