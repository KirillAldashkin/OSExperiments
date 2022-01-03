[bits 16]
WriteLine:            ; Вывод строки на экран с переводом строки
	pusha
	call Write        ; Просто вызываем метод Write...
	mov ah, 0x0e      ; ...и дополнительно выводим "\r\n"
	mov al, 0x0a
	int 0x10
	mov al, 0x0d
	int 0x10
	popa              ; Возврат
	ret

Write:                ; Вывод строки на экран
	pusha
	mov ah, 0x0e
WriteSTART:           ; В 'bx' передаётся указатель на начало строки
	mov al, [bx]      ; Получаем символ из памяти
	cmp al, 0         ; Прерываем цикл, если конец строки
	je WriteRET
	int 0x10          ; Выводим текущий символ
	inc bx            ; Переходим к следующему символу
	jmp WriteSTART    ; Цикл
WriteRET:
	popa
	ret

WriteHex:             ; Вывод 16-ричного числа (в 'bx') на экран
	pusha
	mov ah, 0x0e
	mov cl, 0
WriteHexLOOP:
	cmp cl, 4         ; Повторяем цикл 4 раза
	je WriteHexEND
	rol bx, 4         ; Сдвигаем числа влево на один 16-ричный знак (4 бита)
	mov al, bl        ; Копируем этот 16-ричный знак в 'al', чтобы...
	and al, 0x0f      ; Преобразовать в ASCII символ
	add al, '0'
	cmp al, '9'
	jle WriteHexSKIP
	add al, 7
WriteHexSKIP:
	int 10h           ; Выводим преобразованный символ
	inc cl
	jmp WriteHexLOOP  ; Цикл
WriteHexEND:
	popa
	ret

[bits 32]
WriteProt:            ; Вывод строки на экран, но в защищённом режиме (прерывания недоступны)
	pusha
	mov edx, 0xb8000  ; Видеопамять находится по адресу 0xb8000
	mov ah, 0x0f
WriteProtLOOP:        ; В 'ebx' передаётся указатель на начало строки
	mov al, [ebx]     ; Получаем символ из памяти
	cmp al, 0         ; Прерываем цикл, если конец строки
	je WriteProtEND
	mov [edx], ax     ; Пишем наш символ ('al') и аттрибуты ('ah') в видеопямять
    add ebx, 1        ; Переходим к следующему символу в строке...
    add edx, 2        ;                             ...и в видеопамяти
	jmp WriteProtLOOP ; Цикл
WriteProtEND:
	popa
	ret