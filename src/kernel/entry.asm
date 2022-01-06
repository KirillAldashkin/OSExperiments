; Точка входа в ядро. Вызывает функцию 'main'
[bits 32]
[extern KernelEntry]
global _start
_start:
	call KernelEntry
	ret