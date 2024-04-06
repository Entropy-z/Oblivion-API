#include <Windows.h>
#include <structs.h>
#include <common.h>

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
