; Функция вывода строки на экран в реальном режиме.

%ifndef UTILS_WRITE
%define UTILS_WRITE
	
%macro write 1
	push bx
	mov bx, %1
	call WriteFunc
	pop bx
%endmacro

jmp SkipWriteFunc          ; Защита от случайного выполнения функции
	WriteFunc:
		pusha
		mov ah, 0x0E
	WriteFuncSTART:        ; В 'bx' передаётся указатель на начало строки
		mov al, [bx]       ; Получаем символ из памяти
		cmp al, 0          ; Прерываем цикл, если конец строки
		je WriteFuncRET
		int 0x10           ; Выводим текущий символ
		inc bx             ; Переходим к следующему символу
		jmp WriteFuncSTART ; Цикл
	WriteFuncRET:
		popa
		ret
SkipWriteFunc:
%endif