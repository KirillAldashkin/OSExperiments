; Простой MBR сектор, который ищет первый активный раздел, считывает по (CHS 
; адресации) и грузится с него. В целом совместим с другими ОС'ями.

[bits 16]
[org 0x7E00]
BootCode:
	; BIOS загрузит нас (MBR) по адресу 0x7C00, но VBR надо загрузить по этому
	; же адресу, то есть поверх нас. Поэтому сначала копируем себя в другое
	; место (например, по адресу 0x7E00) и передаём управление нашей копии.
	; Настраиваем регистры
	mov ax, 0x0000
	mov ss, ax
	mov ds, ax
	mov es, ax
	; Настраиваем стэк
	mov bp, 0x7BF0
	mov sp, bp

	mov si, 0x7C00     ; Откуда копировать
	mov di, 0x7E00     ; Куда копировать
	mov cx, 0x200      ; Сколько копировать
	rep movsb          ; Копируем
	jmp ActualBootCode ; Прыгаем

ActualBootCode:
	%include "write.asm"
	write SearchActivePart
	mov cx, 0               ; Номер раздела (от 0 до 3)
	mov bx, Part1Attributes ; Указывает на аттрибуты раздела
	WhileStart:
		; Проверяем, закончились ли разделы
		cmp cx, 4
		jl NotYet
	NoMorePartitions:
		; Закончились :(
		write NoActiveParts
		cli
		hlt
	NotYet:
		inc byte [CheckingStr+19] ; Обновляем номер раздела в строке
		write CheckingStr
		; Проверяем атрибуты раздела
		mov dh, [bx]
		and dh, 0x80
		cmp dh, 0x80
		jne NotActivePartition
	ActivePartition:
		write BootingStr
		mov ah, 0x02        ; Читаем
		mov al, 1           ; 1 сектор

		; Задаём CHS адрес
		inc bx              ; Теперь BX указывает на номер головки
		mov dh, [bx]
		inc bx              ; Теперь BX указывает на номер сектора
		mov cl, [bx]
		inc bx              ; Теперь BX указывает на номер цилиндра
		mov ch, [bx]

		mov bx, 0x7C00      ; На место загрузочного сектора в памяти (ES уже настроен)
		int 0x13            ; Читаем
		jc ErrorReading
		jmp 0x7C00          ; Прыгаем
	ErrorReading:
		; Что-то пошло не так при чтении
		write ReadErrorStr
		cli
		hlt
	NotActivePartition:
		; Этот раздел не подошёл, переходим к следующему
		inc cx
		; Добавляем размер записи о разделе (16 байт) к BX чтобы перейти к следующему
		add bx, 16
		jmp WhileStart
	
	BootDrive: db 0
	BootingStr: db "Booting...", 13, 10, 0
	CheckingStr: db "Checking partition 0...", 13, 10, 0
	ReadErrorStr: db "Could not read VBR.", 0
	NoActiveParts: db "No active partitions found.", 0
	SearchActivePart: db "Simple MBR by Aldashkin Kirill (github.com/KirillAldashkin/OSExperiments).", 13, 10, "Searching active partition...", 13, 10, 0

Padding:
	times 440-($-$$) db 0xFF

; Структура разделов MBR. При записи на диск будет заменена на настоящие значения.
DiskSignature:
	db "AAAA"
Reserved:
	db "BB"
PartEntry1:
	Part1Attributes: db "1"
	Part1CHSStart:   db "222"
	Part1Type:       db "3"
	Part1CHSEnd:     db "444"
	Part1LBAStart:   db "5555"
	Part1Length:     db "6666"
PartEntry2:
	Part2Attributes: db "1"
	Part2CHSStart:   db "222"
	Part2Type:       db "3"
	Part2CHSEnd:     db "444"
	Part2LBAStart:   db "5555"
	Part2Length:     db "6666"
PartEntry3:
	Part3Attributes: db "1"
	Part3CHSStart:   db "222"
	Part3Type:       db "3"
	Part3CHSEnd:     db "444"
	Part3LBAStart:   db "5555"
	Part3Length:     db "6666"
PartEntry4:
	Part4Attributes: db "1"
	Part4CHSStart:   db "222"
	Part4Type:       db "3"
	Part4CHSEnd:     db "444"
	Part4LBAStart:   db "5555"
	Part4Length:     db "6666"
BootableSignature:
	dw 0x55AA