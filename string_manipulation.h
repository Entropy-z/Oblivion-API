#pragma once

#include <Windows.h>   

SIZE_T WCharStringToCharString(_Inout_ PCHAR Destination, _In_ PWCHAR Source, _In_ SIZE_T MaximumAllowed)
{
	INT Length = (INT)MaximumAllowed;

	while (--Length >= 0)
	{
#pragma warning( push )
#pragma warning( disable : 4244)
		if (!(*Destination++ = *Source++))
			return MaximumAllowed - Length - 1;
#pragma warning( pop ) 
	}

	return MaximumAllowed - Length;
}

SIZE_T CharStringToWCharString(_Inout_ PWCHAR Destination, _In_ PCHAR Source, SIZE_T _In_ MaximumAllowed)
{
	INT Length = (INT)MaximumAllowed;

	while (--Length >= 0)
	{
		if (!(*Destination++ = *Source++))
			return MaximumAllowed - Length - 1;
	}

	return MaximumAllowed - Length;
}

SIZE_T StringLengthA(_In_ LPCSTR String)
{
	LPCSTR String2;

	for (String2 = String; *String2; ++String2);

	return (String2 - String);
}

SIZE_T StringLengthW(_In_ LPCWSTR String)
{
	LPCWSTR String2;

	for (String2 = String; *String2; ++String2);

	return (String2 - String);
}

INT StringCompareA(_In_ LPCSTR String1, _In_ LPCSTR String2)
{
	for (; *String1 == *String2; String1++, String2++)
	{
		if (*String1 == '\0')
			return 0;
	}

	return ((*(LPCSTR)String1 < *(LPCSTR)String2) ? -1 : +1);
}

INT StringCompareW(_In_ LPCWSTR String1, _In_ LPCWSTR String2)
{
	for (; *String1 == *String2; String1++, String2++)
	{
		if (*String1 == '\0')
			return 0;
	}

	return ((*(LPCWSTR)String1 < *(LPCWSTR)String2) ? -1 : +1);
}

void toLowerCaseChar(char* str) {
	while (*str) {
		if (*str >= 'A' && *str <= 'Z') {
			*str = *str + ('a' - 'A');
		}
		str++;
	}
}

WCHAR toLowerCaseWchar(WCHAR ch) {
	if (ch >= L'A' && ch <= L'Z') {
		return ch + (L'a' - L'A');
	}
	return ch;
}

wchar_t* _strcpy(wchar_t* dest, const wchar_t* src)
{
	wchar_t* p;

	if ((dest == NULL) || (src == NULL))
		return dest;

	if (dest == src)
		return dest;

	p = dest;
	while (*src != 0) {
		*p = *src;
		p++;
		src++;
	}

	*p = 0;
	return dest;
}

wchar_t* _strcat(wchar_t* dest, const wchar_t* src)
{
	if ((dest == NULL) || (src == NULL))
		return dest;

	while (*dest != 0)
		dest++;

	while (*src != 0) {
		*dest = *src;
		dest++;
		src++;
	}

	*dest = 0;
	return dest;
}

BOOL IsStringEqual (IN LPCWSTR Str1, IN LPCWSTR Str2) {

	WCHAR	lStr1	[MAX_PATH],
			lStr2	[MAX_PATH];

	int		len1	= lstrlenW(Str1),
			len2	= lstrlenW(Str2);

	int		i		= 0,
			j		= 0;

	if (len1 >= MAX_PATH || len2 >= MAX_PATH)
		return FALSE;

	for (i = 0; i < len1; i++){
		lStr1[i] = (WCHAR)tolower(Str1[i]);
	}
	lStr1[i++] = L'\0'; // null terminating


	for (j = 0; j < len2; j++) {
		lStr2[j] = (WCHAR)tolower(Str2[j]);
	}
	lStr2[j++] = L'\0'; // null terminating


	if (lstrcmpiW(lStr1, lStr2) == 0)
		return TRUE;

	return FALSE;
}

#define PRINTA( STR, ... )                                                                  \
    if (1) {                                                                                \
        LPSTR buf = (LPSTR)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 1024 );           \
        if ( buf != NULL ) {                                                                \
            int len = wsprintfA( buf, STR, __VA_ARGS__ );                                   \
            WriteConsoleA( GetStdHandle( STD_OUTPUT_HANDLE ), buf, len, NULL, NULL );       \
            HeapFree( GetProcessHeap(), 0, buf );                                           \
        }                                                                                   \
    }  

#define PRINTW( STR, ... )                                                                  \
    if (1) {                                                                                \
        LPWSTR buf = (LPWSTR)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 1024 );         \
        if ( buf != NULL ) {                                                                \
            int len = wsprintfW( buf, STR, __VA_ARGS__ );                                   \
            WriteConsoleW( GetStdHandle( STD_OUTPUT_HANDLE ), buf, len, NULL, NULL );       \
            HeapFree( GetProcessHeap(), 0, buf );                                           \
        }                                                                                   \
    }  