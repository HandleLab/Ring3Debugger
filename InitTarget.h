#include <windows.h>
#include <iostream>

DWORD findTarget();

struct debugValue
{
	DWORD targetAddress;
	DWORD dr7Mask;
};

debugValue PromptDebugValue();

bool SetThreadContextExtraction(DWORD dwThreadId, debugValue DebugValue, CONTEXT ctx);