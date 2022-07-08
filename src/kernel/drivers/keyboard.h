// Драйвер PS/2-совместимой клавиатуры.
#ifndef DRIVERS_KEYBOARD_H
#define DRIVERS_KEYBOARD_H

#include "../utils/types.h"

// Не-ASCII клавиши
#define Escape -1
#define Backspace -2
#define LControl -3
#define LShift -4
#define RShift -5
#define PadStar -6
#define LAlt -7
#define CapsLock -8

typedef void(*CharHandlerCallback)(bool, char);

// Инициализирует драйвер клавиатуры.
void InitKeyboard();
// Устанавливает обработчик события клавиши.
void SetCharHandler(CharHandlerCallback handler);
// Ожидает событие нажатия клавиши.
void WaitKey();

#endif