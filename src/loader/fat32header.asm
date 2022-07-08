[org 0x7c00]

; Структура заголовка FAT32. 
; Значения будут заменены настоящими после записи на диск.
; BIOS Parameter Block
JumpCode:
	jmp BootCode
	nop
OEMIdentifier:		db "00000000"
BytesPerSector:		db "11"
SectorsPerCluster:	db "2"
ReservedSectors:	db "33"
FATCount:			db "4"
RootDirEntries:		db "55"
TotalSectors:		db "66"
MediaType:			db "7"
SectorsPerFAT:		db "88"
SectorsPerTrack:	db "99"
HeadsCount:			db "AA"
HiddenSectors:		db "BBBB"
LargeSectorCount:	db "CCCC"

; Extended BIOS Parameter Block
SectorsPerFAT32:	db "DDDD"
Flags:				db "EE"
FATVersion:			db "FF"
RootDirCluster:		db "GGGG"
FSInfoSector:		db "HH"
BackupBPBSector:	db "JJ"
Reserved:			db "KKKKKKKKKKKK"
DriveNumber:		db "L"
NTFlags:			db "M"
Signature:			db "N"
VolumeID:			db "OOOO"
VolumeLabel:		db "PPPPPPPPPPP"
SystemIdentifier:	db "QQQQQQQQ"
BootCode:
	%include "loader\fat32code.asm"
	times 510-($-$$) db 0x00
BootableSignature:	dw 0xAA55