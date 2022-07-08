; Определяет доступные пространства операптивной памяти 
; и записывает сведения по адресу [0x7FF00;0x80000).

MemMapSegment equ 0x7FF0
%include "write.asm"

; Segment - 7FF0; [Offset...Size]:
;
; 0x00...1  - количество отрезков
; 0x01...15 - не используется
; 0x10...4  - начало отрезка 1
; 0x14...4  - конец отрезка 1
; 0x18...4  - начало отрезка 2
; 0x1C...4  - конец отрезка 2
; ...
; 0xF0...4 - начало отрезка 29
; 0xF4...4 - конец отрезка 29
; 0xF8...4 - начало отрезка 30
; 0xFC...4 - конец отрезка 30

ProbeVar1:
	; TODO - "дырки" в адресах ОЗУ и способы их определения - сплошной
	; зоопарк. Надо добавить больше споособов их определения.
ProbeVar2:
	write PrbMemory
	; Поиск дырок с помощью прерывания 0x15 при AX=0xE801. 
	; Поддерживается почти везде.
	mov cx, 0
	mov dx, 0
	mov ax, 0xE801
	clc
	int 0x15
	jnc Mem2_Ok
		; Этот способ не поддерживается.
		write ProbeError
		cli
		hlt
	Mem2_Ok:
		; В AX или CX находится количество ОЗУ (делённое на 1024) начиная
		; с 0x100000 и заканчивая потенциальной дыркой для ISA устройств.
		; Удостоверяемся, что это число будет именно в AX.
		cmp ax, 0
		jne AXNotZero
			mov ax, cx
		AXNotZero:
		; В BX или DX находится количество ОЗУ (делённое на 65536) начиная
		; с 0x1000000 и заканчивая потенциальной дыркой для фиг знает чего.
		; Удостоверяемся, что это число будет именно в BX.
		cmp bx, 0
		jne BXNotZero
			mov bx, dx
		BXNotZero:
		push ds ; Запоминаем DS на всякий случай
		push MemMapSegment
		pop ds
		; Всего один или два отрезка - [1..15]MB и, возможно, [16..]MB
		mov byte [ds:0x00], 1
		; Начало первого отрезка - 1MB
		mov word [ds:0x10], 0x0000
		mov word [ds:0x12], 0x0010
		; Конец первого отрезка - 1MB + AX блоков по 1KB (размер в байтах >> 10)
		mov word [ds:0x14], ax
		shl word [ds:0x14], 10
		mov word [ds:0x16], ax
		shr word [ds:0x16], 6 ; <<10 + >>16 = >>6
		add word [ds:0x16], 0x0010
		; А после 0x1000000 точно есть память?
		cmp bx, 0
		je NoMoreBX
			; Всё-таки есть
			inc byte [ds:0x00]
			; Начало второго отрезка - 16MB
			mov word [ds:0x18], 0x0000
			mov word [ds:0x1A], 0x0100
			; Конец второго отрезка - 16MB + BX блоков по 64KB (размер в байтах >> 16)
			mov word [ds:0x1C], 0x0000 ; Сюда из BX ничего не попадет
			mov word [ds:0x1E], bx
			add word [ds:0x1E], 0x0100
		NoMoreBX:
		pop ds ; Восстанавливаем DS
		jmp ProbeEnd

PrbMemory: db 'Probing memory...', 13, 10, 0
ProbeError: db 'Could not probe memory. Halt', 13, 10, 0
PrbEndMsg:  db 'End probing memory.', 13, 10, 0

ProbeEnd:
	write PrbEndMsg