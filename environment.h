#pragma once

#include <Windows.h>
#include <structs.h>
#include <string_manipulation.h>

PPEB GetPeb(){

#if _WIN64
	return (PPEB)(__readgsqword(0x60));
#elif _WIN32
	return (PPEB)(__readfsdword(0x30));
#endif

	return NULL;
}

VOID PrintAllEnvValues() {

	PPEB pPeb = NULL;
	if ((pPeb = GetPeb()) == NULL)
		return;
    
	PBYTE pTmp = (PBYTE)pPeb->ProcessParameters->Environment;
    
	while (1)
	{
		int j = StringLengthW((LPCWSTR)pTmp);

		if (!j) {
			pTmp = NULL;
			break;
		}
		PRINTW(L"%s \n\n", pTmp);

		pTmp = (PBYTE)pTmp + (j * sizeof(WCHAR)) + sizeof(WCHAR);
	}
}

DWORD GetNumberOfProcessors() { 

	PPEB pPeb = NULL;
	if ((pPeb = GetPeb()) == NULL)
		return NULL;

	PBYTE	pTmp = (PBYTE)pPeb->ProcessParameters->Environment;

	while (1){
		int j = lstrlenW((LPCWSTR)pTmp);

		if (!j) {
			pTmp = NULL;
			break;
		}

		if (*(ULONG_PTR*)pTmp == *(ULONG_PTR*)L"NUMBER") // NUMBER_OF_PROCESSORS
			break;

		pTmp = (PBYTE)pTmp + (j * sizeof(WCHAR)) + sizeof(WCHAR);
	}


	if (pTmp) {
		int j = lstrlenW((LPCWSTR)pTmp) * sizeof(WCHAR);

		for (int i = 0; i <= j; i++) {
			if ((WCHAR)pTmp[i] == (WCHAR)L'=')
				return (DWORD)wcstoul((PWSTR)&pTmp[i + sizeof(WCHAR)], NULL, 10);
		}
	}
	return NULL;
}

PWSTR GetAppDataPath() {

	PPEB pPeb = NULL;
	if ((pPeb = GetPeb()) == NULL)
		return NULL;

	PBYTE		pTmp = (PBYTE)pPeb->ProcessParameters->Environment;

	while (1)
	{
		int j = lstrlenW((LPCWSTR)pTmp);

		if (!j) {
			pTmp = NULL;
			break;
		}

		if (*(ULONG_PTR*)pTmp == *(ULONG_PTR*)L"APPDATA")
			break;
		
		pTmp = (PBYTE)pTmp + (j * sizeof(WCHAR)) + sizeof(WCHAR);
	}

	if (pTmp) {
		int j = StringLengthW((LPCWSTR)pTmp) * sizeof(WCHAR);

		for (int i = 0; i <= j; i++) {
			if ((WCHAR)pTmp[i] == (WCHAR)L'=')
				return (PWSTR)&pTmp[i + sizeof(WCHAR)];  // skipping the equal sign
		}
	}

	return NULL;
}

PWSTR GetTmpPath() {

	PPEB pPeb = NULL;
	if ((pPeb = GetPeb()) == NULL)
		return NULL;

	PBYTE		pTmp = (PBYTE)pPeb->ProcessParameters->Environment;

	while (1){
		int j = StringLengthW((LPCWSTR)pTmp);

		if (!j) {
			pTmp = NULL;
			break;
		}

		if (*(ULONG_PTR*)pTmp == *(ULONG_PTR*)L"TEMP")
			break;

		pTmp = (PBYTE)pTmp + (j * sizeof(WCHAR)) + sizeof(WCHAR);
	}


	if (pTmp) {
		int j = StringLengthW((LPCWSTR)pTmp) * sizeof(WCHAR);

		for (int i = 0; i <= j; i++) {
			if ((WCHAR)pTmp[i] == (WCHAR)L'=')
				return (PWSTR)&pTmp[i + sizeof(WCHAR)]; // skipping the equal sign
		}
	}
	
	return NULL;
}

PWSTR GetWinDirPath() {

	PPEB pPeb = NULL;
	if ((pPeb = GetPeb()) == NULL)
		return NULL;

	PBYTE		pTmp = (PBYTE)pPeb->ProcessParameters->Environment;

	while (1)
	{
		int j = lstrlenW((LPCWSTR)pTmp);

		if (!j) {
			pTmp = NULL;
			break;
		}

		if (*(ULONG_PTR*)pTmp == *(ULONG_PTR*)L"windir")
			break;

		pTmp = (PBYTE)pTmp + (j * sizeof(WCHAR)) + sizeof(WCHAR);
	}


	if (pTmp) {
		int j = lstrlenW((LPCWSTR)pTmp) * sizeof(WCHAR);
		
		for (int i = 0; i <= j; i++) {
			if ((WCHAR)pTmp[i] == (WCHAR)L'=')
				return (PWSTR)&pTmp[i + sizeof(WCHAR)]; 
		}
	}
	return NULL;
}
