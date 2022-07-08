// Драйвер внутреннего таймера.
#ifndef DRIVERS_TIMER_H
#define DRIVERS_TIMER_H

#include "../utils/types.h"

// Инициализирует таймер.
void InitTimer(uint32 frequency);
// Выполняет задержку на указанное число миллисекунд.
void Delay(uint32 millis);
// Возвращает количество прошедших тиков.
uint32 GetTimerTicks();
// Возвращает частоту таймера.
uint32 GetTimerFrequency();

#endif