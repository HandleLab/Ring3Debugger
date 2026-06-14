#include "InitTarget.h"

bool closeDll = false;

DWORD GetMainThreadId()
{
	HWND windowHanddle{ NULL };
	std::wstring windowName;

	std::cout << "Window name : ";
	std::wcin >> windowName;
	
	windowHanddle = FindWindowW(NULL, windowName.c_str());

	if (windowHanddle != NULL)
	{
		DWORD mainThreadID{ NULL };

		mainThreadID = GetWindowThreadProcessId(windowHanddle, NULL);

		if (mainThreadID != NULL)
		{
			std::cout << "[+] Main Thread found !" << std::endl;
			std::cout << "[+] Main Thread ID : " << mainThreadID << std::endl;
			return mainThreadID;
		}
		
		std::cout << "[-] Main thread not found..." << std::endl;
	}

	std::cout << "[-] Hwnd not found..." << std::endl;
	return NULL;
}



debugValue PromptDebugValue()
{
	debugValue registerDebugValue{0,0};

	std::cout << "Enter target adress for register Dr0 : ";
	std::cin >> std::hex >> registerDebugValue.targetAddress;

	std::cout << "Enter Dr7 mask : ";
	std::cin >> std::hex >> registerDebugValue.dr7Mask;

	return registerDebugValue;
}



bool SetThreadContextExtraction(debugValue DebugValue)
{
	
	DWORD threadId = GetMainThreadId();


	if (threadId != NULL)
	{
		HANDLE threadHdl = OpenThread(THREAD_ALL_ACCESS, NULL, threadId);

		if (threadHdl != NULL)
		{
			std::cout << "[+] Thread handle set !" << std::endl;

			SuspendThread(threadHdl);

			CONTEXT ctx = { 0 };
			ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

			GetThreadContext(threadHdl, &ctx);

			ctx.Dr0 = DebugValue.targetAddress;
			ctx.Dr7 = DebugValue.dr7Mask;
			
			SetThreadContext(threadHdl, &ctx);
			ResumeThread(threadHdl);
			
			CloseHandle(threadHdl);
			return true;
		}

	}
	
	return false;
}


LONG CALLBACK MyVehHandler(PEXCEPTION_POINTERS ExceptionInfo)
{
	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
	{
		std::cout << "Adresse du network serializer : " << std::hex << ExceptionInfo->ContextRecord->Eip << std::endl;

		ExceptionInfo->ContextRecord->Dr0 = 0;
		ExceptionInfo->ContextRecord->Dr7 = 0;

		closeDll = true;

		return EXCEPTION_CONTINUE_EXECUTION;
	}

	else
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
}