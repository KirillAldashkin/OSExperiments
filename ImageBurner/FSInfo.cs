using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential, Pack = 1)]
unsafe internal struct FSInfo
{
    public uint Signature1; //0x41615252
    public fixed byte Reserved1[480];
    public uint Signature2; //0x61417272
    public uint LastFreeSector;
    public uint FirstSector;
    public fixed byte Reserved2[12];
    public uint Signature3; //0xAA550000
}