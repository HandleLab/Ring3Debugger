#include <windows.h>
#include <iostream>

#define CALL_FIRST 1 

extern bool closeDll;

DWORD GetMainThreadId();

struct debugValue
{
	DWORD targetAddress;
	DWORD dr7Mask;
};

debugValue PromptDebugValue();

bool SetThreadContextExtraction(debugValue DebugValue);

LONG CALLBACK MyVehHandler(PEXCEPTION_POINTERS ExceptionInfo);