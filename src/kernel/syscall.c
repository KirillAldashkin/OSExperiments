#include "syscall.h"
#include "drivers/screen.h"
#include "libc/stdlib.h"
#include "libc/types.h"
#include "libc/mem.h"
#include <stdint.h>

void SystemCall(Registers* regs)
{
	void* ptr = (void*)(regs->Ebx);
	// Тестовый код, потом заменим на полезные запросы
	uint16_t code = *(uint16_t*)ptr;
	if(code == 0) {
		EchoCall* xptr = (EchoCall*)ptr;
		xptr->To = xptr->From;
	}
}