#ifndef IAT_H
#define IAT_H

#include <Windows.h>

typedef HMODULE (WINAPI* fnLoadLibraryA)(IN LPCSTR lpLibFileName);

VOID IatCamouflage();
UINT32 CRC32B(LPCSTR cString);

HMODULE GetModuleHandleH(IN UINT32 uModuleHash) ;
FARPROC GetProcAddressH(IN HMODULE hModule, IN UINT32 uApiHash);

FARPROC GetProcAddressNoHashing(IN HMODULE hModule, IN LPCSTR lpApiName);
HMODULE GetModuleHandleNoHashing(IN LPCWSTR szModuleName);

#endif