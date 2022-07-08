#include "syscall.h"

void SystemCall(Registers* regs)
{
	void* ptr = (void*)(regs->Ebx);
	uint16 code = *(uint16*)ptr;
	// Тестовый код, потом заменим на полезные запросы
	if(code == 0) {
		EchoCall* xptr = (EchoCall*)ptr;
		xptr->To = xptr->From;
	}
}