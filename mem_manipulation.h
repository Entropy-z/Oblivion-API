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

VOID ZeroMemoryEx(_Inout_ PVOID Destination, _In_ SIZE_T Size)
{
	PULONG Dest = (PULONG)Destination;
	SIZE_T Count = Size / sizeof(ULONG);

	while (Count > 0)
	{
		*Dest = 0;
		Dest++;
		Count--;
	}

	return;
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
