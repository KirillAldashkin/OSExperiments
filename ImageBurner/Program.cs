using System.Runtime.CompilerServices;

// Получаем информацию
var diskPath = args[0];                        // Путь к файлу образа диска
var mbrPath = args[1];                         // Путь к файлу MBR сектора
var vbrPath = args[2];                         // Путь к файлу VBR сектора
var diskOffset = Convert.ToInt64(args[3], 16); // Смещение в байтах начала раздела в образе диска
var loaderPath = args[4];                      // Путь к файлу ядра

// Считываем фалйы
var newMbr = Utils.Read<MBR>(mbrPath);
var newBpb = Utils.Read<BPB>(vbrPath);
var loaderData = File.ReadAllBytes(loaderPath);
Console.WriteLine($"Размер ядра: {loaderData.Length} байт");

// Считываем MBR, BPB и FSInfo
var partFile = new FileStream(diskPath, FileMode.Open, FileAccess.ReadWrite);

var mbr = partFile.Read<MBR>();
partFile.Position = diskOffset; 
var bpb = partFile.Read<BPB>();
partFile.Position = diskOffset + bpb.FSInfoSector * bpb.BytesPerSector;
var fsinfo = partFile.Read<FSInfo>();

// Проверяем, влезет ли ядро между описанием ФС и FAT'ами
var reservedSize = (bpb.ReservedSectors - 3) * bpb.BytesPerSector; // 3 сектора зарезервированы - BPB, FSInfo и бэкап BPB
if(reservedSize < loaderData.Length)
{
    Console.WriteLine("Ядро не влезет между описанием ФС и FAT'ами. Отформатируйте раздел иначе или уменьшите ядро.");
    return;
}

// Обновляем сведения о ФС
bpb.FSInfoSector = 1;
bpb.BackupBootSector = 2;
unsafe
{
    Unsafe.CopyBlockUnaligned(bpb.JumpAsm, newBpb.JumpAsm, 3);
    Unsafe.CopyBlockUnaligned(mbr.BootCode, newMbr.BootCode, 440);
    Unsafe.CopyBlockUnaligned(bpb.BootCode, newBpb.BootCode, 420);
}

// Записываем диск
partFile.Position = 0;
partFile.Write(mbr);
partFile.Position = diskOffset;
partFile.Write(bpb); // BPB+EBPB
partFile.Write(fsinfo); // FSInfo
partFile.Write(bpb); // бэкап BPB+EBPB
partFile.Write(loaderData);
partFile.Close();

static unsafe class Utils
{
    public static T Read<T>(this Stream from) where T : unmanaged
    {
        T ret = default;
        byte* ptr = (byte*)&ret;
        from.Read(new Span<byte>(ptr, sizeof(T)));
        return ret;
    }

    public static void Write<T>(this Stream to, T data) where T : unmanaged
    {
        byte* ptr = (byte*)&data;
        to.Write(new Span<byte>(ptr, sizeof(T)));
    }

    public static T Read<T>(string path) where T : unmanaged
    {
        using var stream = new FileStream(path, FileMode.Open, FileAccess.Read);
        var data = stream.Read<T>();
        return data;
    }
}