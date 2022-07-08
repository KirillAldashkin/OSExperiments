// Обрабатывает клавиши от драйвера клавиатуры и 
// упрощает создание комадной оболочки.
#ifndef SHELL_H
#define SHELL_H

#include "utils/types.h"

typedef void(*InputHandler)(string);

// Инициализирует командную оболочку.
void ShellInit(InputHandler nhandler);
#endif