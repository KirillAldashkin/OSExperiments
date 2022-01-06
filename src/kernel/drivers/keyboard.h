#pragma once

#include "../types.h"

typedef void(*StringHandlerCallback)(string);
typedef void(*CharHandlerCallback)(char);

void InitKeyboard();
void SetCharHandler(CharHandlerCallback handler);
void SetStringHandler(StringHandlerCallback handler);