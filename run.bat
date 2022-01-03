@echo off

rem Установить кодировку UTF8
chcp 65001

rem Путь к папке с QEMU. Нужна, так как QEMU сама не знает, где она установлена (странно...)
set QemuDir=C:\Program Files\qemu

rem Всякие префиксы для инструментов GNU
set Pre=tools\bin\i686-elf-
set GCC=%Pre%gcc
set GDB=%Pre%gdb
set LD=%Pre%ld

rem Объектные файлы
set KernelObjC=build\obj\kernelC.elf
set KernelObjAsm=build\obj\kernelAsm.elf
set KernelElf=build\\obj\\kernel.elf
rem Бинарники
set BootBin=build\bin\bootsect.bin
set KernelBin=build\bin\kernel.bin
set BootImage=build\bootimg.bin

echo "||| Очистка |||"
del %BootBin% %KernelObjC% %KernelElf% %KernelObjAsm% %KernelBin% %BootImage%
mkdir build\bin
mkdir build\obj

echo "||| Компиляция загрузочного сектора |||"
cd src\bootsect
nasm -f bin boot.asm -o ..\..\%BootBin%
cd ..\..

echo "||| Компиляция ядра (Assembler) |||"
cd src\kernel
nasm entry.asm -f elf32 -o ..\..\%KernelObjAsm%

echo "||| Компиляция ядра (C) |||"
..\..\%GCC% -m32 -ffreestanding -g -c kernel.c -o ..\..\%KernelObjC%
cd ..\..

echo "||| Линковка ядра |||"
%LD% -melf_i386 -o %KernelBin% -Ttext 0x7e00 %KernelObjAsm% %KernelObjC% --oformat binary

echo "||| Линковка ядра для отладки |||"
%LD% -melf_i386 -o %KernelElf% -Ttext 0x7e00 %KernelObjAsm% %KernelObjC%

echo "||| Создание загрузочного образа |||"
copy /B %BootBin%+%KernelBin% %BootImage%

echo "||| Запуск QEMU |||"
qemu-system-i386 -fda %BootImage% -L "%QemuDir%"

echo "||| Запуск GDB |||"
%GDB% -ex "symbol-file %KernelElf%"