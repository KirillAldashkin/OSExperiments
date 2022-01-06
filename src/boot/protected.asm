[bits 16]
EnterProMode:
    cli                         ; 1) Выключаем прерывания
    lgdt [GDTDescriptor]        ; 2) Загружаем GDT
    mov eax, cr0                ; 3) Устанавливаем 32 битный режим
    or eax, 0x1
    mov cr0, eax
    jmp CodeSegment:InitProMode ; 4) far jump. Зачем - пока не разобрался

[bits 32]
InitProMode:
    mov ax, DataSegment         ; 5) Обновляем сегменты
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000         ; 6) Обновляем стэк
    mov esp, ebp
    call StartProMode        ; 7) Переходим к основному коду