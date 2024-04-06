#ifndef COMMON_H
#define COMMON_H

#include <structs.h>
#include <Windows.h>

/*--------------------------------------------------[ processes and threads ]--------------------------------------------------*/

PPEB GetPeb(){

#if _WIN64
	return (PPEB)(__readgsqword(0x60));
#elif _WIN32
	return (PPEB)(__readfsdword(0x30));
#endif

	return NULL;
}

PTEB GetTeb(VOID)
{
#if defined(_WIN64)
	return (PTEB)__readgsqword(0x30);
#elif defined(_WIN32)
	return (PTEB)__readfsdword(0x18);
#endif
}

DWORD _GetCurrentProcessId();

DWORD _GetCurrentThreadId();

#define NtCurrentProcess() ((HANDLE)-1) 
#define NtCurrentThread()  ((HANDLE)-2) 

HANDLE GetCurrentProcessNoForward(VOID);
HANDLE GetCurrentThreadNoForward(VOID);

HANDLE GetProcessHeapFromTeb(VOID);

PWSTR GetCmdLine(OPTIONAL OUT PSIZE_T pSize);

PWSTR GetCurrentDir(OPTIONAL OUT PSIZE_T pSize);

/*---------------------------------------------------[ memory manipulation ]---------------------------------------------------*/

extern void* __cdecl memset(void*, int, size_t);

PVOID CopyMemoryEx(_Inout_ PVOID Destination, _In_ CONST PVOID Source, _In_ SIZE_T Length);

VOID ZeroMemoryEx(_Inout_ PVOID Destination, _In_ SIZE_T Size);


/*---------------------------------------------------[ string manipulation ]---------------------------------------------------*/

SIZE_T WCharStringToCharString(_Inout_ PCHAR Destination, _In_ PWCHAR Source, _In_ SIZE_T MaximumAllowed);

SIZE_T CharStringToWCharString(_Inout_ PWCHAR Destination, _In_ PCHAR Source, SIZE_T _In_ MaximumAllowed);

SIZE_T StringLengthA(_In_ LPCSTR String);

SIZE_T StringLengthW(_In_ LPCWSTR String);

INT StringCompareA(_In_ LPCSTR String1, _In_ LPCSTR String2);

INT StringCompareW(_In_ LPCWSTR String1, _In_ LPCWSTR String2);

void toLowerCaseChar(char* str);

WCHAR toLowerCaseWchar(WCHAR ch);

PCHAR StringCopyA(_Inout_ PCHAR String1, _In_ LPCSTR String2);

PWCHAR StringCopyW(_Inout_ PWCHAR String1, _In_ LPCWSTR String2);

WCHAR StringConcatW(_Inout_ PWCHAR String, _In_ LPCWSTR String2);

PCHAR StringConcatA(_Inout_ PCHAR String, _In_ LPCSTR String2);

BOOL IsStringEqual (IN LPCWSTR Str1, IN LPCWSTR Str2);

/*--------------------------------------------------------[ internet ]---------------------------------------------------------*/



/*--------------------------------------------------[ Environment Variables ]--------------------------------------------------*/

PWSTR GetWinDirPath();

PWSTR GetTmpPath();

PWSTR GetAppDataPath();

DWORD GetNumberOfProcessors();

VOID PrintAllEnvValues();

#endif