; Строит карту оперативной памяти, переходит в защищённый 
; 32-битный режим и передаёт управление C коду.

[extern KernelEntry] ; Точка входа в C код
[bits 16]

global _start ; Для линковщика
_start:
	%include "kernel\memory_prober.asm"  ; Для построения карты оперативной памяти
	%include "kernel\protected_mode.asm" ; Для перехода в защищённый режим
	call KernelEntry ; Вызываем C код
Halt:                ; Ядро (почему-то) завершило свою работу, зависаем
	hlt
	jmp Halt