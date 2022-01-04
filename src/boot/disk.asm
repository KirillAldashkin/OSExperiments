DiskLoad:                       ; Функция считывания с диска
	pusha
	push dx                     ; В 'dh' передаётся число секторов для считывания. Пока запомним их
	mov ah, 0x02                ; 'ah' - функция прерывания 13h. 0x02 - это чтение
	mov al, dh                  ; 'al' - количество секторов для считывания. Передаётся через 'dh'
    mov cl, 0x02                ; 'cl' - первый сектор считывания. 0x01 - загрузочный, 0x02 - первый доступный
    mov ch, 0x00                ; 'ch' - номер цилиндра
                                ; 'dl' - номер устройства. Передаётся вызывающим методом
    mov dh, 0x00                ; 'dh' - номер головки
                                ; [es:bx] - указатель на буфер. Передаётся вызывающим методом
	int 0x13                    ; Считываем
	jc ReadError                ; Произошла ли ошиюка при считывании?
	pop dx
    cmp al, dh                  ; (Прерывание 0x13 передает в 'al' количество считанных секторов)
    jne SectorCountError        ; Все необходимые секторы были прочитаны?
	popa                        ; Возврат
	ret

SectorCountError:               ; Если не совпало количество секторов, то...
	mov bx, SectorCountErrorStr ; ...выводим ошибку...
	call Write
	jmp $                       ; ...и зависаем

ReadError:                      ; Если произошла ошибка, то...
	mov bx, ReadErrorStr        ; ...выводим сообщение,...
	call Write
	mov bh, ah                  ; ...код ошибки (передаётся в 'ah'),...
	mov bl, dl                  ; ...и номер диска (мы сами установили его в 'dl')
	call WriteHex
	jmp $

ReadErrorStr: db 'Disk read error. Code|Disk: ', 0
SectorCountErrorStr: db 'Incorrect number of sectors read. ', 0