; Глобальная таблица дескрипторов. Я с ней пока не разобрался :)
GDTStart:
    dd 0x0
    dd 0x0
GDTCode: 
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
GDTData:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
GDTEnd:

GDTDescriptor:
    dw GDTEnd - GDTStart - 1
    dd GDTStart

CodeSegment equ GDTCode - GDTStart
DataSegment equ GDTData - GDTStart