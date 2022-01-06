[org 0x7c00]
KernOffs equ 0x7e00

Start:                   ; Точка входа
	mov [BootDisk], dl   ; Запоминаем устройство, с которого загрузились
	mov bx, 0x8fc0       ; Настраиваем стэк: 0x9fc00 - 0x10 == 0x9fbf0 == 0x8fc0:fff0
	mov ss, bx
	mov bp, 0xfff0
	mov sp, bp
	mov bx, RealModeStr  ; Выводим сообщение
	call WriteLine
	call LoadKernel      ; Грузим ядро в оперативку
	call EnterProMode    ; Уходим в защищённый режим

LoadKernel:
	mov bx, LoadKernStr  ; Выводим сообщение
	call WriteLine
	mov bx, KernOffs     ; По "этому" адресу...
	mov dh, 31           ; ...необходимо прочитать 31 сектор...
	mov dl, [BootDisk]   ; ...с "этого" устройства.
	call DiskLoad        ; Грузим!
	ret

%include "disk.asm"      ; Утилиты для работы с диском
%include "print.asm"     ; Утилиты для печати
%include "protected.asm" ; Утилиты для перехода в защищённый режим
%include "gdt.asm"       ; GDT для перехода в защищённый режим

[bits 32]
StartProMode:            ; Точка входа в защищённый режим
	mov ebx, ProtModeStr ; Выводим сообщение
	call WriteProt
	call 0x7e00          ; Передаём управление ядру
	jmp $

BootDisk: db 0
RealModeStr: db 'In real mode', 0
LoadKernStr: db 'Loading kernel', 0
ProtModeStr: db 'In protected mode', 0

times 510-($-$$) db 0
dw 0xaa55