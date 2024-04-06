#pragma once

#include <Windows.h>
#include <mem_manipulation.h>
#include <process_threads.h>
#include <string_manipulation.h>


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/

BOOL IsPathValidA(_In_ LPCSTR FilePath){
	HANDLE hFile = INVALID_HANDLE_VALUE;

	hFile = CreateFileA(FilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	if (hFile)
		CloseHandle(hFile);

	return TRUE;
}

BOOL IsPathValidW(_In_ LPCWSTR FilePath){
	HANDLE hFile = INVALID_HANDLE_VALUE;

	hFile = CreateFileW(FilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	if (hFile)
		CloseHandle(hFile);

	return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/

BOOL ReadFileFromDisk(LPCSTR lpFileName, PBYTE* pFile, SIZE_T* sFile) {

	HANDLE	hFile = INVALID_HANDLE_VALUE;
	PBYTE	pBuff = NULL;
	DWORD	dwFileSize = NULL,
		dwNumberOfBytesRead = NULL;

	PRINTA("[i] Reading \"%s\"...\n", lpFileName);

	hFile = CreateFileA(lpFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		PRINTA("\t[!] CreateFileA for file Failed With Error : %d \n", GetLastError());
		goto _CLEANUP;
	}

	PRINTA("\t[*] CreateFile for file Successfully\n");

	dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == NULL) {
		PRINTA("\t[!] GetFileSize for file Failed With Error : %d \n", GetLastError());
		goto _CLEANUP;
	}

	pBuff = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
	if (pBuff == NULL) {
		PRINTA("\t[!] HeapAlloc for file Failed With Error : %d \n", GetLastError());
		goto _CLEANUP;
	}

    PRINTA("\t[*] Allocated buffer of file at: 0x%p\n\n", pBuff);

	if (!ReadFile(hFile, pBuff, dwFileSize, &dwNumberOfBytesRead, NULL) || dwFileSize != dwNumberOfBytesRead) {
		PRINTA("\t[!] ReadFile for binary file Failed With Error : %d \n", GetLastError());
		goto _CLEANUP;
	}
	PRINTA("\t[!] Bytes Read for binary file: %d of : %d \n\n", dwNumberOfBytesRead, dwFileSize);


_CLEANUP:
	*pFile = (PBYTE)pBuff;
	*sFile = (SIZE_T)dwFileSize;
	if (hFile)
		CloseHandle(hFile);
	if (*pFile == NULL || *sFile == NULL)
		return FALSE;
	return TRUE;
}

