#pragma once

#include <Windows.h>
#include <stdio.h>
#include <winternl.h>
#include <string_manipulation.h>

FARPROC GetProcAddressReplacement(IN HMODULE hModule, IN LPCSTR lpApiName) {

	PBYTE pBase = (PBYTE)hModule;

	PIMAGE_DOS_HEADER	pImgDosHdr		= (PIMAGE_DOS_HEADER)pBase;
	if (pImgDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
		return NULL;

	PIMAGE_NT_HEADERS	pImgNtHdrs		= (PIMAGE_NT_HEADERS)(pBase + pImgDosHdr->e_lfanew);
	if (pImgNtHdrs->Signature != IMAGE_NT_SIGNATURE)
		return NULL;

	IMAGE_OPTIONAL_HEADER	ImgOptHdr	= pImgNtHdrs->OptionalHeader;

	PIMAGE_EXPORT_DIRECTORY pImgExportDir = (PIMAGE_EXPORT_DIRECTORY) (pBase + ImgOptHdr.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	PDWORD FunctionNameArray = (PDWORD)(pBase + pImgExportDir->AddressOfNames);

	PDWORD FunctionAddressArray = (PDWORD)(pBase + pImgExportDir->AddressOfFunctions);

	PWORD  FunctionOrdinalArray = (PWORD)(pBase + pImgExportDir->AddressOfNameOrdinals);

	for (DWORD i = 0; i < pImgExportDir->NumberOfFunctions; i++){

		CHAR* pFunctionName = (CHAR*)(pBase + FunctionNameArray[i]);

		FARPROC pFunctionAddress = (FARPROC)(pBase + FunctionAddressArray[FunctionOrdinalArray[i]]);

		if (strcmp(lpApiName, pFunctionName) == 0){
			//printf("[ %0.4d ] FOUND API -\t NAME: %s -\t ADDRESS: 0x%p  -\t ORDINAL: %d\n", i, pFunctionName, pFunctionAddress, FunctionOrdinalArray[i]);
			return pFunctionAddress;
		}
	}

	return NULL;
}

HMODULE GetModuleHandleReplacement(IN LPCWSTR szModuleName) {

#ifdef _WIN64 // if compiling as x64
	PPEB					pPeb		= (PEB*)(__readgsqword(0x60));
#elif _WIN32 // if compiling as x32
	PPEB					pPeb		= (PEB*)(__readfsdword(0x30));
#endif

	PPEB_LDR_DATA			pLdr		= (PPEB_LDR_DATA)(pPeb->Ldr);
	PLDR_DATA_TABLE_ENTRY	pDte		= (PLDR_DATA_TABLE_ENTRY)(pLdr->InMemoryOrderModuleList.Flink);
	
	while (pDte) {
		
		if (pDte->FullDllName.Length != NULL) {

			if (IsStringEqual(pDte->FullDllName.Buffer, szModuleName)) {
				wprintf(L"[+] Found Dll \"%s\" \n", pDte->FullDllName.Buffer);
#ifdef STRUCTS
				return (HMODULE)(pDte->InInitializationOrderLinks.Flink);
#else
				return (HMODULE)pDte->Reserved2[0];
#endif // STRUCTS

			}

			// wprintf(L"[i] \"%s\" \n", pDte->FullDllName.Buffer);
		}
		else {
			break;
		}
		
		// next element in the linked list
		pDte = *(PLDR_DATA_TABLE_ENTRY*)(pDte);

	}

	return NULL;
}

HMODULE GetModuleHandleReplacement2(IN LPCWSTR szModuleName) {

#ifdef _WIN64
	PPEB					pPeb				= (PEB*)(__readgsqword(0x60));
#elif _WIN32
	PPEB					pPeb				= (PEB*)(__readfsdword(0x30));
#endif

	PLDR_DATA_TABLE_ENTRY	pDte				= (PLDR_DATA_TABLE_ENTRY)(pPeb->Ldr->InMemoryOrderModuleList.Flink);
	
	PLIST_ENTRY				pListHead			= (PLIST_ENTRY)&pPeb->Ldr->InMemoryOrderModuleList;
	PLIST_ENTRY				pListNode			= (PLIST_ENTRY)pListHead->Flink;

	do
	{
		if (pDte->FullDllName.Length != NULL) {
			if (IsStringEqual(pDte->FullDllName.Buffer, szModuleName)) {
				//wprintf(L"[+] Found Dll \"%s\" \n", pDte->FullDllName.Buffer);
#ifdef STRUCTS
				return (HMODULE)(pDte->InInitializationOrderLinks.Flink);
#else
				return (HMODULE)pDte->Reserved2[0];
#endif // STRUCTS
			}

			//wprintf(L"[i] \"%s\" \n", pDte->FullDllName.Buffer);

			pDte = (PLDR_DATA_TABLE_ENTRY)(pListNode->Flink);

			pListNode = (PLIST_ENTRY)pListNode->Flink;

		}

	} while (pListNode != pListHead);



	return NULL;
}


