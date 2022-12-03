#include "drivers/screen.h"
#include "drivers/timer.h"
#include "drivers/keyboard.h"
#include "drivers/ide.h"
#include "utils/types.h"
#include "utils/strings.h"
#include "hardware/idt.h"
#include "hardware/isr.h"
#include "hardware/pci.h"
#include "memorymap.h"
#include "shell.h"
#include "fs/drives.h"
#include "fs/parts.h"

void HandleCommand(string cmd);

void KernelEntry() {
	ClearScreen();
	WriteLine("Hello ^-^!");

	SetupInterrupts();
	SetupIRQ();
	ShellInit(HandleCommand);
	InitSectorsCache();
	InitIDE((uint32[5]) { 0x1F0, 0x3F6, 0x170, 0x376, 0x000 });
}

void PrintTreePartition(FSEntry* entry, uint16 ident);
void PrintMemMap();
void PrintHelp();
void PrintPCIDevice(uint8 bus, uint8 slot, uint8 func, PCIHeaderCommon* id);

void HandleCommand(string cmd) {
	if (StringCompare(cmd, "memmap") == 0) PrintMemMap();
	else if (StringCompare(cmd, "listpci") == 0) PCIEnumerateDevices(PrintPCIDevice);
	else if (StringCompare(cmd, "clear") == 0) ClearScreen();
	else if (StringCompare(cmd, "help") == 0) PrintHelp();
	else if (StringCompare(cmd, "crash") == 0) { volatile int b = 0; b /= b; }
	else if (StringCompare(cmd, "filetree") == 0) PrintTreePartition(&Parts[0].root, 0);
	else WriteLine("Unknown command.");
}

void PrintHelp() {
	WriteLine("help     - prints this help.");
	WriteLine("clear    - clears screen.");
	WriteLine("crash    - crashes system.");
	WriteLine("memmap   - prints memory map.");
	WriteLine("listpci  - lists all PCI devices.");
	WriteLine("filetree - prints file tree of a first partition on a first disk.");
}

void PrintSpaces(uint16 count) {
	while (count-- > 0) WriteChar(' ');
}
void PrintTreePartition(FSEntry* entry, uint16 ident) {
	FSEntryEnumerator enumr = entry->dir.getEnumerator(entry);
	while (enumr.moveNext(&enumr)) {
		char name[12];
		name[11] = '\0';
		enumr.current.getName(&enumr.current, name, 11);
		if (StringCompare(name, ".          ") == 0) continue;
		if (StringCompare(name, "..         ") == 0) continue;

		PrintSpaces(ident);
		if (enumr.current.type == PT_EntryFile)
			Write("FILE ");
		else
			Write("DIR  ");

		Write("\"");
		Write(name);
		Write("\"");
		if (enumr.current.type == PT_EntryFile) {
			Write(" 0x");
			WriteU32(enumr.current.file.getSize(&enumr.current));
			Write(" bytes");
		}

		WriteLine("");
		if (enumr.current.type == PT_EntryDir) {
			PrintTreePartition(&enumr.current, ident + 1);
		}
	}
}

void PrintMemMap() {
	string hdr = "Total entries:    ";
	UIntToString(MemMap->count, hdr+15, 10);
	WriteLine(hdr);
	for(uint8 i = 0; i < MemMap->count; i++) {
		string etr = "  ) [0x        ,0x        )";
		UIntToString(i+1, etr, 10);
		UIntToString(MemMap->entries[i].begin, etr+7, 16);
		UIntToString(MemMap->entries[i].end, etr+18, 16);
		WriteLine(etr);
	}
}

void PrintPCIDevice(uint8 bus, uint8 slot, uint8 func, PCIHeaderCommon* id) {
	Write("BusID:    0x");
	WriteU8(bus);
	Write("       | DeviceID: 0x");
	WriteU8(slot);
	Write("       | FuncID:   0x");
	WriteU8(func);
	NewLine();

	Write("Vendor:   0x");
	WriteU32(id->VendorID);
	Write(" | Device:   0x");
	WriteU32(id->DeviceID);
	Write(" | Header:   0x");
	WriteU32(id->HeaderType);
	NewLine();
	Write("Prog:     0x");
	WriteU32(id->ProgIF);
	Write(" | SubClass: 0x");
	WriteU32(id->SubClassCode);
	Write(" | Class:    0x");
	WriteU32(id->ClassCode);
	NewLine();
	WriteLine("Press any key to continue...");
	WaitKey();
}