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

static uint16 currentPathDepth = 0;
static FSEntry currentPath[128];

void HandleCommand(string cmd);
void WritePath();

void KernelEntry() {
	ClearScreen();
	WriteLine("Hello ^-^!");

	SetupInterrupts();
	SetupIRQ();
	ShellInit(HandleCommand);
	InitSectorsCache();
	InitIDE((uint32[5]) { 0x1F0, 0x3F6, 0x170, 0x376, 0x000 });

	currentPath[0] = Parts[0].root;
	currentPathDepth = 1;
	WritePath();
}

void PrintMemMap();
void PrintHelp();
void EnumerateFiles();
void TypeFile(string name);
void CurrentDirectory(string name);
void PrintPCIDevice(uint8 bus, uint8 slot, uint8 func, PCIHeaderCommon* id);

void HandleCommand(string cmd) {
	if (StringCompare(cmd, "memmap") == 0) PrintMemMap();
	else if (StringCompare(cmd, "listpci") == 0) PCIEnumerateDevices(PrintPCIDevice);
	else if (StringCompare(cmd, "clear") == 0) ClearScreen();
	else if (StringCompare(cmd, "help") == 0) PrintHelp();
	else if (StringCompare(cmd, "crash") == 0) { volatile int b = 0; b /= b; }
	else if (StringCompare(cmd, "dir") == 0) EnumerateFiles();
	else if (StartsWith(cmd, "type ")) TypeFile(cmd + 5);
	else if (StartsWith(cmd, "cd ")) CurrentDirectory(cmd + 3);
	else WriteLine("Unknown command.");
	WritePath();
}

void PrintHelp() {
	WriteLine("help    - prints this help.");
	WriteLine("clear   - clears screen.");
	WriteLine("crash   - crashes system.");
	WriteLine("memmap  - prints memory map.");
	WriteLine("listpci - lists all PCI devices.");
	WriteLine("dir     - lists files in current directory");
	WriteLine("cd      - moves to the directory specified");
	WriteLine("type    - prints specified file");
}

void CurrentDirectory(string name) {
	if (StringCompare(name, ".") == 0) return;
	if (currentPathDepth == 0 || currentPathDepth == 128) return;
	if ((StringCompare(name, "..") == 0)) {
		if(currentPathDepth > 1) currentPathDepth--;
		return;
	}

	FSEntry* cur = currentPath + currentPathDepth - 1;
	FSEntryEnumerator en = cur->dir.getEnumerator(cur);
	while (en.moveNext(&en)) {
		if (en.current.type != PT_EntryDir) continue;
		char inName[12];
		en.current.getName(&en.current, inName, 12);
		if (StringCompare(inName, name) == 0) {
			currentPath[currentPathDepth] = en.current;
			currentPathDepth++;
			return;
		}
	}
	WriteLine("Directory not found.");
}

void TypeFile(string name) {
	FSEntry* cur = currentPath + currentPathDepth - 1;
	FSEntryEnumerator en = cur->dir.getEnumerator(cur);
	while (en.moveNext(&en)) {
		if (en.current.type != PT_EntryFile) continue;
		char inName[12];
		en.current.getName(&en.current, inName, 12);
		if (StringCompare(inName, name) == 0) {
			uint32 size = en.current.file.getSize(&en.current);
			uint8* buffer = (uint8*)0x400000; // TODO temporary
			en.current.file.read(&en.current, 0, size, buffer);
			buffer[size] = '\0';
			WriteLine(buffer);
			return;
		}
	}
	WriteLine("File not found.");
}

void EnumerateFiles() {
	if (currentPathDepth == 0) return;
	FSEntry* cur = currentPath + currentPathDepth - 1;
	FSEntryEnumerator en = cur->dir.getEnumerator(cur);
	while (en.moveNext(&en)) {
		if (en.current.type == PT_EntryFile) {
			Write("FILE | ");
		} else {
			Write("DIR  | ");
		}
		char name[12];
		en.current.getName(&en.current, name, 12);
		Write(name);
		if (en.current.type == PT_EntryFile) {
			char sizeStr[32]; sizeStr[0] = ' ';
			uint32 size = en.current.file.getSize(&en.current);
			uint32 len = UIntLength(size, 10);
			UIntToString(size, sizeStr+1, 10);
			MemoryCopy(sizeStr + 1 + len, " bytes", 7);
			Write(sizeStr);
		}
		WriteLine("");
	}
}

void WritePath() {
	for (uint16 i = 0; i < currentPathDepth; i++) {
		WriteChar('/');
		char name[12];
		name[11] = '\0';
		currentPath[i].getName(&currentPath[i], name, 11);
		Write(name);
	}
	WriteChar('>');
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