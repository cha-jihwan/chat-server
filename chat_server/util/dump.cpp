#include "../common/pre_compile.h"

constexpr size_t c_max_buf_size = 1024;
namespace c2 { namespace server { namespace core
{
	void my_purecall_handler()
	{
		crash();
	}

	void invalid_paramater_handler(const wchar_t* expression, const wchar_t* function, const wchar_t* filfile, unsigned int line, uintptr_t pReserved)
	{
		crash();
	}

	void signal_handler(int error)
	{
		crash();
	}

	LONG WINAPI exception_filter(EXCEPTION_POINTERS* e)
	{
		//size_t invalid_heap_count = 0;

		//unsigned long  heap_count = GetProcessHeaps(0, NULL);
		//HANDLE heaps[200];
		//GetProcessHeaps(heap_count, heaps);
		//PROCESS_HEAP_ENTRY heapEntry;
		//long long sizeSum = 0;

		//for (unsigned long i = 0; i < heap_count; i++)
		//{
		//	if (0 == HeapValidate(heaps[i], 0, NULL))
		//	{
		//		invalid_heap_count += 1;
		//	}
		//}
		//printf(" invalid heap count : %d \n", invalid_heap_count);


		printf("Exception : 0x%08X\r\n",
			e->ExceptionRecord->ExceptionCode);
		printf("Exception Address : 0x%08p\r\n",
			e->ExceptionRecord->ExceptionAddress);


		static uint64_t dump_count = 0;
		uint64_t ret = InterlockedIncrement(&dump_count);

		_invalid_parameter_handler oldHandler, newHandler;
		newHandler = invalid_paramater_handler;

		oldHandler = _set_invalid_parameter_handler(newHandler);	// crt 함수에 null 포인터 등을 넣었을 때....
		_CrtSetReportFile(_CRT_WARN, 0);							// CRT 오류 메시지 표시 중단. 바로 덤프로 남도록.
		_CrtSetReportFile(_CRT_ASSERT, 0);							// CRT 오류 메시지 표시 중단. 바로 덤프로 남도록.
		_CrtSetReportFile(_CRT_ERROR, 0);							// CRT 오류 메시지 표시 중단. 바로 덤프로 남도록.

		//_CrtSetReportHook(_custom_Report_hook);

		//---------------------------------------------------------------------------
		// pure virtual function called 에러 핸드러를 사용자 정의 함수로 우회시킨다.
		//---------------------------------------------------------------------------
		_set_purecall_handler(my_purecall_handler);

		_set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

		signal(SIGABRT, signal_handler);
		signal(SIGINT, signal_handler);
		signal(SIGILL, signal_handler);
		signal(SIGFPE, signal_handler);
		signal(SIGSEGV, signal_handler);
		signal(SIGTERM, signal_handler);



		TCHAR tszFileName[c_max_buf_size] = { 0 };
		SYSTEMTIME stTime = { 0 };
		GetSystemTime(&stTime);
		StringCbPrintf(tszFileName,
			_countof(tszFileName),
			_T("%s_%4d%02d%02d_%02d%02d%02d_%d.dmp"),
			_T("Dump"),
			stTime.wYear,
			stTime.wMonth,
			stTime.wDay,
			stTime.wHour,
			stTime.wMinute,
			stTime.wSecond,
			ret);



		HANDLE hFile = CreateFile(tszFileName, GENERIC_WRITE, /*FILE_SHARE_READ*/ FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("Dump Error : %d \n", GetLastError());
			return EXCEPTION_EXECUTE_HANDLER;
		}
		MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
		exceptionInfo.ThreadId = GetCurrentThreadId();
		exceptionInfo.ExceptionPointers = e;
		exceptionInfo.ClientPointers = FALSE;

		MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(MiniDumpWithPrivateReadWriteMemory |
			MiniDumpWithDataSegs | MiniDumpWithHandleData | MiniDumpWithFullMemoryInfo |
			MiniDumpScanMemory | MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules | MiniDumpWithFullMemory);

		// MiniDumpWriteDump를 사용하여 hFile에 덤프 기록
		MiniDumpWriteDump(GetCurrentProcess()
			, GetCurrentProcessId()
			, hFile
			, mdt//(MINIDUMP_TYPE)(MiniDumpWithPrivateReadWriteMemory | MiniDumpScanMemory | MiniDumpWithFullMemory)
			, (e != nullptr) ? &exceptionInfo : 0
			, 0
			, NULL);

		if (hFile)
		{
			CloseHandle(hFile);
			hFile = NULL;
		}

		return EXCEPTION_EXECUTE_HANDLER;
	}
}
}
}