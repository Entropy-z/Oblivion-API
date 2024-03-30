#pragma once
#include <Windows.h>

PVOID CopyMemoryEx(_Inout_ PVOID Destination, _In_ CONST PVOID Source, _In_ SIZE_T Length)
{
	PBYTE D = (PBYTE)Destination;
	PBYTE S = (PBYTE)Source;

	while (Length--)
		*D++ = *S++;

	return Destination;
}

/*--------------------------------------------------------------------------------------*/

extern void* __cdecl memset(void*, int, size_t);

#pragma intrinsic(memset)

#pragma function(memset)

void* __cdecl memset(void* Destination, int Value, size_t Size) {
	unsigned char* p = (unsigned char*)Destination;
	while (Size > 0) {
		*p = (unsigned char)Value;
		p++;
		Size--;
	}
	return Destination;
}
