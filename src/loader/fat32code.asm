MaxSectorsRead equ 127
%include "write.asm"

; Готовим стэк
mov bx, 0x0000
mov ss, bx
mov bp, 0x7BF0
mov sp, bp
write LoadingStr

; Откуда считываем: HiddenSectors - начало раздела
mov ax, [HiddenSectors]    ; Младшие 2 байта
mov [DAPReadOffsetLow], ax
mov ax, [HiddenSectors+2]  ; Старшие 2 байта
mov [DAPReadOffsetLow+2], ax
; Добавляем 1 сектор, чтобы пропустить BPB+EBPB
clc
inc word [DAPReadOffsetLow] ; Младшие 2 байта
jnc ResOverflowEnd
	inc word [DAPReadOffsetLow+2] ; Старшие 2 байта
ResOverflowEnd:

; Сколько считываем: ReservedSectors - все сектора до начала FAT'ов
mov ax, [ReservedSectors]
; Некоторые вредные BIOS'ы не читают больше 127 секторов. 
; Впрочем, это 63.5КБ, так что нам хватит.
cmp ax, 127
jle SkipCalcLength
	mov ax, 127
SkipCalcLength:
mov [DAPReadLength], al

; Номер функции и параметры
mov ah, 0x42
mov bx, 0
mov ds, bx
mov si, DAP

; Читаем и прыгаем
int 0x13 
write JumpingStr
jmp 0x8200 ; 0x7E00 + 0x200 (FSInfo) + 0x200 (PBR Backup) = 0x8200
DAP:
	DAPLength: db 0x10
	DAPReserved: db 0
	DAPReadLength: dw 0
	DAPTargetOffset: dw 0x0000
	DAPTargetSegment: dw 0x07E0 ; 0x07E0:0x0000 = 0x00007E00
	DAPReadOffsetLow: dd 0
	DAPReadOffsetHigh: dd 0

LoadingStr: db "Reading loader...", 13, 10, 0
JumpingStr: db "Launching loader...", 13, 10, 0