; Код для обработки прерываний
[bits 32]

; Вызываемые функции C
[extern ISRHandler]
[extern IRQHandler]
[extern SystemCall]

CommonISRHandler:
	pusha           ; Пушим edi, esi, ebp, esp, ebx, edx, ecx, eax
	mov ax, ds      ; Пушим ds
	push eax
	mov ax, 0x08    ; 0x08 - KernelCodeSegment в нашем GDT, устанавливаем его
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
    push esp
    cld
	call ISRHandler ; Вызываем обработчик
    pop eax
    pop eax         ; Возвращаем всё, как было
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8      ; Убираем из стэка код прерывания и номер ошибки (4+4=8 байт)
	iretd           ; Убираем из стэка cs, eip, eflags, ss, esp (процессор запушил их автоматически)

; Почти тоже самое, что и CommonISRHandler, но чутка отличается
CommonIRQHandler:
	pusha
	mov ax, ds
	push eax
	mov ax, 0x08
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
    push esp
	cld
    call IRQHandler
    pop ebx
    pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	popa
	add esp, 8
	iretd

; Обработка системного вызова
global SysCall
SysCall:
	push byte 0
	push byte 0x80
    pusha
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
    push esp
    call SystemCall
    pop ebx
    pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	popa
	add esp, 8
	iretd

; Мы не можем отличать прерывания друг от друга, поэтому нам
; придётся делать для каждого по обработчику. Так же некоторые
; прерывания имеют код ошибки, а некоторые - нет. Для таких мы
; будем пушить 0, чтобы аргументы не отличались
global Isr0
global Isr1
global Isr2
global Isr3
global Isr4
global Isr5
global Isr6
global Isr7
global Isr8
global Isr9
global Isr10
global Isr11
global Isr12
global Isr13
global Isr14
global Isr15
global Isr16
global Isr17
global Isr18
global Isr19
global Isr20
global Isr21
global Isr22
global Isr23
global Isr24
global Isr25
global Isr26
global Isr27
global Isr28
global Isr29
global Isr30
global Isr31
; IRQs
global Irq0
global Irq1
global Irq2
global Irq3
global Irq4
global Irq5
global Irq6
global Irq7
global Irq8
global Irq9
global Irq10
global Irq11
global Irq12
global Irq13
global Irq14
global Irq15

; Деление на ноль
Isr0:
    push byte 0
    push byte 0
    jmp CommonISRHandler
; Отладка
Isr1:
    push byte 0
    push byte 1
    jmp CommonISRHandler
; Неигнорируемое прерывание (???)
Isr2:
    push byte 0
    push byte 2
    jmp CommonISRHandler
; Прерывание 3
Isr3:
    push byte 0
    push byte 3
    jmp CommonISRHandler
; Ошибка INTO
Isr4:
    push byte 0
    push byte 4
    jmp CommonISRHandler
; Выход за границы
Isr5:
    push byte 0
    push byte 5
    jmp CommonISRHandler
; Неверный код операции (op-code)
Isr6:
    push byte 0
    push byte 6
    jmp CommonISRHandler
; Сопроцессор недоступен
Isr7:
    push byte 0
    push byte 7
    jmp CommonISRHandler
; Ошибка при обработке другой ошибки или прерывания
Isr8:
    ; Имеет свой код, пушить не надо
    push byte 8
    jmp CommonISRHandler
; Ошика доступа к памяти у сопроцессора
Isr9:
    push byte 0
    push byte 9
    jmp CommonISRHandler
; Ошибка TSS
Isr10:
    ; Имеет свой код, пушить не надо
    push byte 10
    jmp CommonISRHandler
; Несуществуюший сегмент
Isr11:
    ; Имеет свой код, пушить не надо
    push byte 11
    jmp CommonISRHandler
; Ошибка стэка
Isr12:
    ; Имеет свой код, пушить не надо
    push byte 12
    jmp CommonISRHandler
; Общая ошибка (???)
Isr13:
    ; Имеет свой код, пушить не надо
    push byte 13
    jmp CommonISRHandler
; Ошибка страниц памяти
Isr14:
    ; Имеет свой код, пушить не надо
    push byte 14
    jmp CommonISRHandler
; Зарезервировано
Isr15:
    push byte 0
    push byte 15
    jmp CommonISRHandler
; Ошибка в числе с плавающей запятой
Isr16:
    push byte 0
    push byte 16
    jmp CommonISRHandler
; Ошибка при проверке выравнивания
Isr17:
    push byte 0
    push byte 17
    jmp CommonISRHandler
; Ошибка на аппаратном уровне 
Isr18:
    push byte 0
    push byte 18
    jmp CommonISRHandler
; Зарезервировано
Isr19:
    push byte 0
    push byte 19
    jmp CommonISRHandler
; Зарезервировано
Isr20:
    push byte 0
    push byte 20
    jmp CommonISRHandler
; Зарезервировано
Isr21:
    push byte 0
    push byte 21
    jmp CommonISRHandler
; Зарезервировано
Isr22:
    push byte 0
    push byte 22
    jmp CommonISRHandler
; Зарезервировано
Isr23:
    push byte 0
    push byte 23
    jmp CommonISRHandler
; Зарезервировано
Isr24:
    push byte 0
    push byte 24
    jmp CommonISRHandler
; Зарезервировано
Isr25:
    push byte 0
    push byte 25
    jmp CommonISRHandler
; Зарезервировано
Isr26:
    push byte 0
    push byte 26
    jmp CommonISRHandler
; Зарезервировано
Isr27:
    push byte 0
    push byte 27
    jmp CommonISRHandler
; Зарезервировано
Isr28:
    push byte 0
    push byte 28
    jmp CommonISRHandler
; Зарезервировано
Isr29:
    push byte 0
    push byte 29
    jmp CommonISRHandler
; Зарезервировано
Isr30:
    push byte 0
    push byte 30
    jmp CommonISRHandler
; Зарезервировано
Isr31:
    push byte 0
    push byte 31
    jmp CommonISRHandler

; IRQ прерывания
Irq0:
	push byte 0
	push byte 32
    jmp CommonIRQHandler
Irq1:
	push byte 0
	push byte 33
    jmp CommonIRQHandler
Irq2:
	push byte 0
	push byte 34
    jmp CommonIRQHandler
Irq3:
	push byte 0
	push byte 35
    jmp CommonIRQHandler
Irq4:
	push byte 0
	push byte 36
    jmp CommonIRQHandler
Irq5:
	push byte 0
	push byte 37
    jmp CommonIRQHandler
Irq6:
	push byte 0
	push byte 38
    jmp CommonIRQHandler
Irq7:
	push byte 0
	push byte 39
    jmp CommonIRQHandler
Irq8:
	push byte 0
	push byte 40
    jmp CommonIRQHandler
Irq9:
	push byte 0
	push byte 41
    jmp CommonIRQHandler
Irq10:
	push byte 0
	push byte 42
    jmp CommonIRQHandler
Irq11:
	push byte 0
	push byte 43
    jmp CommonIRQHandler
Irq12:
	push byte 0
	push byte 44
    jmp CommonIRQHandler
Irq13:
	push byte 0
	push byte 45
    jmp CommonIRQHandler
Irq14:
	push byte 0
	push byte 46
    jmp CommonIRQHandler
Irq15:
	push byte 0
	push byte 47
    jmp CommonIRQHandler