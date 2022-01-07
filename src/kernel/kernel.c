#include "hardware/isr.h"
#include "hardware/timer.h"
#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "libc/string.h"
#include "libc/stdlib.h"
#include "libc/mem.h"
#include "syscall.h"
#include "char.h"

void Command(string cmd);
void Char(char c);

void KernelEntry() {
    ClearScreen();
    SetupInterrupts();
    SetupIRQ();
    SetCharHandler(Char);
    SetStringHandler(Command);
    WriteLine("Welcome to... need to come up with a name...");
    WriteLine("Write 'help' to get more info.");
}

void Char(char c) {
    if (c == 0) {
        Back();
    } else {
        WriteChar(c);
    }
}

const string helpstr = "\
help - prints this message\n\
clear - clears the screen\n\
ticks - prints tick count\n\
crash - divides by zero :)";

void Command(string cmd) {
    ToSmallStr(cmd);
    NewLine();
    if (strcmp("help", cmd) == 0) {
        WriteLine(helpstr);
    } else if (strcmp("clear", cmd) == 0) {
        ClearScreen();
    } else if (strcmp("ticks", cmd) == 0) {
        string res = "            ";
        memset(res, ' ', 12);
        itoa(GetTicks(), res, 10);
        WriteLine(res);
    } else if (strcmp("crash", cmd) == 0) {
        volatile int a = 0;
        volatile int b = 0;
        b = a / b;
    } else if (strcmp("hello", cmd) == 0) {
        WriteLine("Hi! ^-^");
    } else if (strcmp("syscall", cmd) == 0) {
        EchoCall call;
        call.MustBeZero = 0;
        call.From = 0xABCDEF;
        SysCall(call);
        string s = "      ";
        itoa(call.To, s, 16);
        WriteLine(s);
    } else {
        Write("Unknown command: \"");
        Write(cmd);
        WriteLine("\"");
    }
}