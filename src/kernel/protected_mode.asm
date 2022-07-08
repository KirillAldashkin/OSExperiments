; Переход в защищённый режим
KernelCode equ GDTEntry_KernelCode - GDT ; Номер сегмента кода ядра
KernelData equ GDTEntry_KernelData - GDT ; Номер сегмента данных ядра

PreparePM:
	[bits 16]
	; 1) Выключаем прерывания
	cli
	; 2) Загружаем GDT
	lgdt [GDTDesc]
	; 3) Устанавливаем бит защищённого режима
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	; 4) Прыгаем "далеко", чтобы процессор понял, что произошло
	jmp KernelCode:FinishPM

; Описание GDT
GDTDesc:
	dw GDT_End - GDT - 1
	dd GDT

; Запись в GDT:
; dw Лимит [00-15]
; dw База  [00-15]
; db База  [16-23]
; db Флаги доступа
; db Флаги (4 бита) + лимит (4 бита) [16-19]
; db База  [24-31]
GDT:
	GDTEntry_Null:
		dd 0x0
		dd 0x0
	GDTEntry_KernelCode: ; Код ядра
		dw 0xFFFF     
		dw 0x0        
		db 0x0        
		db 0b10011010 
		db 0b11001111
		db 0x0        
	GDTEntry_KernelData: ; Данные ядра
		dw 0xFFFF
		dw 0x0
		db 0x0
		db 0b10010010
		db 0b11001111
		db 0x0
	GDTEntry_UserCode: ; Код пользователя
		dw 0xFFFF
		dw 0x0
		db 0x0
		db 0b11111010
		db 0b11001111
		db 0x0
	GDTEntry_UserData: ; Данные пользователя
		dw 0xFFFF
		dw 0x0
		db 0x0
		db 0b11110010
		db 0b11001111
		db 0x0
; TODO - Возможно, TSS сегмент для многозадачности
GDT_End:

FinishPM:
	[bits 32]
	; 5) Обновляем сегменты
	mov ax, KernelData
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	; 6) ???
	; 7) PROFIT!