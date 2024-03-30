#pragma once

#include <Windows.h>
#include <structs.h>

PTEB GetTeb(VOID)
{
#if defined(_WIN64)
	return (PTEB)__readgsqword(0x30);
#elif defined(_WIN32)
	return (PTEB)__readfsdword(0x18);
#endif
}

PPEB GetPeb () {

#if _WIN64
	return (PPEB)(__readgsqword(0x60));
#elif _WIN32
	return (PPEB)(__readfsdword(0x30));
#endif

	return NULL;
}

/*-------------------------------------------------------------------------------------------*/

DWORD _GetCurrentProcessId() {

#if _WIN64
	return (DWORD)(__readgsdword(0x40));
#elif _WIN32
	return (DWORD)(__readfsdword(0x20));
#endif

	return NULL;
}

DWORD _GetCurrentThreadId() {

#if _WIN64
	return (DWORD)(__readgsdword(0x48));
#elif _WIN32
	return (DWORD)(__readfsdword(0x24));
#endif

	return NULL;
}

/*-------------------------------------------------------------------------------------------*/

#define NtCurrentProcess() ((HANDLE)-1) // Return the pseudo handle for the current process
#define NtCurrentThread()  ((HANDLE)-2) // Return the pseudo handle for the current thread

HANDLE GetCurrentProcessNoForward(VOID)
{
	return (HANDLE)((HANDLE)-1);
}

HANDLE GetCurrentThreadNoForward(VOID)
{
	return ((HANDLE)(LONG_PTR)-2);
}

/*-------------------------------------------------------------------------------------------*/

HANDLE GetProcessHeapFromTeb(VOID)
{
	return GetPeb()->ProcessHeap;
}

/*-------------------------------------------------------------------------------------------*/

PWSTR GetCmdLine(OPTIONAL OUT PSIZE_T pSize) {
	
	PPEB pPeb = NULL;
	if ((pPeb = GetPeb()) == NULL)
		return NULL;

	if (pSize)
		*pSize = (SIZE_T)pPeb->ProcessParameters->CommandLine.Length;
	
	return (PWSTR)pPeb->ProcessParameters->CommandLine.Buffer;
}

PWSTR GetCurrentDir(OPTIONAL OUT PSIZE_T pSize) {
	PPEB pPeb = NULL;
	if ((pPeb = GetPeb()) == NULL)
		return NULL;

	if (pSize)
		*pSize = (SIZE_T)pPeb->ProcessParameters->CurrentDirectory.DosPath.Length;

	return (PWSTR)pPeb->ProcessParameters->CurrentDirectory.DosPath.Buffer;
}

/*-------------------------------------------------------------------------------------------*/
