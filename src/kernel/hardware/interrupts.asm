[bits 32]

[extern ISRHandler]     ; Вызываемые функции
[extern IRQHandler]

CommonISRHandler:
	pusha               ; Пушим edi, esi, ebp, esp, ebx, edx, ecx, eax
	mov ax, ds          ; Пушим ds
	push eax
	mov ax, 0x10        ; 0x10 - CodeSegment в нашем GDT, устанавливаем его
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call ISRHandler     ; Вызываем обработчик
    pop eax             ; Возвращаем всё, как было
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8          ; Убираем из стэка код прерывания и номер ошибки (4+4=8 байт)
	sti
	iretd               ; Убираем из стэка cs, eip, eflags, ss, esp (процессор запушил их автоматически)

; Почти тоже самое, что и CommonISRHandler, чутка отличается
CommonIRQHandler:
	pusha
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call IRQHandler
    pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	popa
	add esp, 8
	sti
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
    cli
    push byte 0
    push byte 0
    jmp CommonISRHandler
; Отладка
Isr1:
    cli
    push byte 0
    push byte 1
    jmp CommonISRHandler
; Неигнорируемое прерывание (???)
Isr2:
    cli
    push byte 0
    push byte 2
    jmp CommonISRHandler
; Прерывание 3
Isr3:
    cli
    push byte 0
    push byte 3
    jmp CommonISRHandler
; Ошибка INTO
Isr4:
    cli
    push byte 0
    push byte 4
    jmp CommonISRHandler
; Выход за границы
Isr5:
    cli
    push byte 0
    push byte 5
    jmp CommonISRHandler
; Неверный код операции (op-code)
Isr6:
    cli
    push byte 0
    push byte 6
    jmp CommonISRHandler
; Сопроцессор недоступен
Isr7:
    cli
    push byte 0
    push byte 7
    jmp CommonISRHandler
; Ошибка при обработке другой ошибки или прерывания
Isr8:
    cli
    ; Имеет свой код, пушить не надо
    push byte 8
    jmp CommonISRHandler
; Ошика доступа к памяти у сопроцессора
Isr9:
    cli
    push byte 0
    push byte 9
    jmp CommonISRHandler
; Ошибка TSS
Isr10:
    cli
    ; Имеет свой код, пушить не надо
    push byte 10
    jmp CommonISRHandler
; Несуществуюший сегмент
Isr11:
    cli
    ; Имеет свой код, пушить не надо
    push byte 11
    jmp CommonISRHandler
; Ошибка стэка
Isr12:
    cli
    ; Имеет свой код, пушить не надо
    push byte 12
    jmp CommonISRHandler
; Общая ошибка (???)
Isr13:
    cli
    ; Имеет свой код, пушить не надо
    push byte 13
    jmp CommonISRHandler
; Ошибка страниц памяти
Isr14:
    cli
    ; Имеет свой код, пушить не надо
    push byte 14
    jmp CommonISRHandler
; Зарезервировано
Isr15:
    cli
    push byte 0
    push byte 15
    jmp CommonISRHandler
; Ошибка в числе с плавающей запятой
Isr16:
    cli
    push byte 0
    push byte 16
    jmp CommonISRHandler
; Ошибка при проверке выравнивания
Isr17:
    cli
    push byte 0
    push byte 17
    jmp CommonISRHandler
; Ошибка на аппаратном уровне 
Isr18:
    cli
    push byte 0
    push byte 18
    jmp CommonISRHandler
; Зарезервировано
Isr19:
    cli
    push byte 0
    push byte 19
    jmp CommonISRHandler
; Зарезервировано
Isr20:
    cli
    push byte 0
    push byte 20
    jmp CommonISRHandler
; Зарезервировано
Isr21:
    cli
    push byte 0
    push byte 21
    jmp CommonISRHandler
; Зарезервировано
Isr22:
    cli
    push byte 0
    push byte 22
    jmp CommonISRHandler
; Зарезервировано
Isr23:
    cli
    push byte 0
    push byte 23
    jmp CommonISRHandler
; Зарезервировано
Isr24:
    cli
    push byte 0
    push byte 24
    jmp CommonISRHandler
; Зарезервировано
Isr25:
    cli
    push byte 0
    push byte 25
    jmp CommonISRHandler
; Зарезервировано
Isr26:
    cli
    push byte 0
    push byte 26
    jmp CommonISRHandler
; Зарезервировано
Isr27:
    cli
    push byte 0
    push byte 27
    jmp CommonISRHandler
; Зарезервировано
Isr28:
    cli
    push byte 0
    push byte 28
    jmp CommonISRHandler
; Зарезервировано
Isr29:
    cli
    push byte 0
    push byte 29
    jmp CommonISRHandler
; Зарезервировано
Isr30:
    cli
    push byte 0
    push byte 30
    jmp CommonISRHandler
; Зарезервировано
Isr31:
    cli
    push byte 0
    push byte 31
    jmp CommonISRHandler

; IRQ прерывания
Irq0:
	cli
	push byte 0
	push byte 32
    jmp CommonIRQHandler
Irq1:
	cli
	push byte 0
	push byte 33
    jmp CommonIRQHandler
Irq2:
	cli
	push byte 0
	push byte 34
    jmp CommonIRQHandler
Irq3:
	cli
	push byte 0
	push byte 35
    jmp CommonIRQHandler
Irq4:
	cli
	push byte 0
	push byte 36
    jmp CommonIRQHandler
Irq5:
	cli
	push byte 0
	push byte 37
    jmp CommonIRQHandler
Irq6:
	cli
	push byte 0
	push byte 38
    jmp CommonIRQHandler
Irq7:
	cli
	push byte 0
	push byte 39
    jmp CommonIRQHandler
Irq8:
	cli
	push byte 0
	push byte 40
    jmp CommonIRQHandler
Irq9:
	cli
	push byte 0
	push byte 41
    jmp CommonIRQHandler
Irq10:
	cli
	push byte 0
	push byte 42
    jmp CommonIRQHandler
Irq11:
	cli
	push byte 0
	push byte 43
    jmp CommonIRQHandler
Irq12:
	cli
	push byte 0
	push byte 44
    jmp CommonIRQHandler
Irq13:
	cli
	push byte 0
	push byte 45
    jmp CommonIRQHandler
Irq14:
	cli
	push byte 0
	push byte 46
    jmp CommonIRQHandler
Irq15:
	cli
	push byte 0
	push byte 47
    jmp CommonIRQHandler
