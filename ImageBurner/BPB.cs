using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential, Pack = 1)]
unsafe internal struct BPB
{
    // BIOS Parameter Block
    public fixed byte JumpAsm[3];
    public fixed byte OEMStringRaw[8];
    public ushort BytesPerSector;
    public byte SectorsPerCluster;
    public ushort ReservedSectors;
    public byte FATCount;
    public ushort RootDirectoryEntries;
    public ushort TotalSectors;
    public byte MediaType;
    public ushort SectorsPerFAT;
    public ushort SectorsPerTrack;
    public ushort Heads;
    public uint HiddenSectors;
    public uint TotalSectorsLarge;
    // Ext. BIOS Parameter Block для FAT32
    public uint SectorsPerFAT32;
    public ushort Flags;
    public ushort FATVersion;
    public uint RootDirectoryClusters;
    public ushort FSInfoSector;
    public ushort BackupBootSector;
    public fixed byte Reserved[12];
    public byte DriveNumber;
    public byte NTFlags;
    public byte Signature;
    public uint VolumeSerial;
    public fixed byte VolumeLabelRaw[11];
    public fixed byte SystemIdentifierRaw[8];
    public fixed byte BootCode[420];
    public ushort BootableSignature;
}