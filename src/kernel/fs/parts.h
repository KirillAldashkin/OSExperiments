#ifndef FS_PARTS_H
#define FS_PARTS_H

#include "../utils/types.h"

#define PT_MaxParts 128

#define PT_Ok            0
#define PT_TooMuchParts  1

#define PT_PartitonFAT32 0xFA32

#define PT_EntryFile 0x77
#define PT_EntryDir  0xAA

typedef struct t_PartitionData PartitionData;
typedef struct t_FSDirectory FSDirectory;
typedef struct t_FSFile FSFile;
typedef struct t_FSEntry FSEntry;
typedef struct t_FSEntryEnumerator FSEntryEnumerator;

typedef FSDirectory(*RootDirGetter)(PartitionData*);
typedef bool(*FSNameGetter)(FSEntry*, string, uint16);
typedef uint32(*FileSizeGetter)(FSEntry*);
typedef uint32(*FileReader)(FSEntry*, uint32, uint32, void*);
typedef FSEntryEnumerator(*EntryEnumGetter)(FSEntry*);
typedef bool(*EntryEnumNext)(FSEntryEnumerator*);

#pragma pack (push, 1)
struct t_FSDirectory {
    EntryEnumGetter getEnumerator;
};

struct t_FSFile {
    FileSizeGetter getSize;
    FileReader read;
};

struct t_FSEntry {
    uint8 type;
    uint16 partition;
    FSDirectory* parent;
    FSNameGetter getName;
    void* data;
    union
    {
        FSDirectory dir;
        FSFile file;
    };
};

struct t_PartitionData {
    void* data;
    uint16 disk;
    uint16 fsType;
    uint32 start;
    uint32 sectors;
    FSEntry root;
};

struct t_FSEntryEnumerator {
    FSDirectory* where;
    void* data;
    FSEntry current;
    EntryEnumNext moveNext;
};
#pragma pack (pop)

PartitionData parts[PT_MaxParts];

uint16 AddFAT32Partition(uint16 disk, uint32 start, uint32 sectors);
#endif