using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential, Pack = 1)]
unsafe internal struct MBR
{
    public fixed byte BootCode[440];
    public int DiskSignature;
    public ushort Reserved;
    public fixed byte Partition1[16];
    public fixed byte Partition2[16];
    public fixed byte Partition3[16];
    public fixed byte Partition4[16];
    public ushort BootableSignature;
}