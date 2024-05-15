#include <windows.h>

PVOID LdrModuleAddr( _In_ LPWSTR ModuleName){

    PTEB                  pTeb  = __readgsqword(0x30);
    PLDR_DATA_TABLE_ENTRY Data  = { 0 };
    PLIST_ENTRY           Head  = { 0 };
    PLIST_ENTRY           Entry = { 0 };

    Head  = &pTeb->ProcessEnvironmentBlock->Ldr->InLoadOrderModuleList;
    Entry = Head->Flink;

    for ( ; Head != Entry ; Entry = Entry->Flink ) {
        Data = C_PTR( Entry );
        if ( wCharCompare(Data->BaseDllName.Buffer, ModuleName ) == 0){
            return C_PTR(Data->DllBase);
        }
    }

    return NULL;
}

PVOID LdrFuncAddr( _In_ PVOID BaseModule, _In_ PCHAR FuncName ) {
    PIMAGE_NT_HEADERS       pImgNt          = { 0 };
    PIMAGE_EXPORT_DIRECTORY pImgExportDir   = { 0 };
    DWORD                   ExpDirSz        =  0x00;
    PDWORD                  AddrOfFuncs     = NULL;
    PDWORD                  AddrOfNames     = NULL;
    PWORD                   AddrOfOrdinals  = NULL;
    PVOID                   FuncAddr        = NULL;

    pImgNt          = C_PTR(BaseModule + ((PIMAGE_DOS_HEADER)BaseModule)->e_lfanew);
    pImgExportDir   = C_PTR(BaseModule + pImgNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    ExpDirSz        = U_PTR(BaseModule + pImgNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size);

    AddrOfNames     = C_PTR(BaseModule + pImgExportDir->AddressOfNames);
    AddrOfFuncs     = C_PTR(BaseModule + pImgExportDir->AddressOfFunctions);
    AddrOfOrdinals  = C_PTR(BaseModule + pImgExportDir->AddressOfNameOrdinals);

    for (int i = 0; i < pImgExportDir->NumberOfNames; i++) {

        PCHAR pFuncName = (PCHAR)(BaseModule + AddrOfNames[i]);
        PVOID pFunctionAddress = C_PTR(BaseModule + AddrOfFuncs[AddrOfOrdinals[i]]);
        
        if (StringCompareA(pFuncName, FuncName) == 0) {
            if ( (U_PTR(pFunctionAddress) >= U_PTR(pImgExportDir) ) &&
                 (U_PTR(pFunctionAddress) <  U_PTR(pImgExportDir) + ExpDirSz) ) {

                CHAR ForwarderName[MAX_PATH] = { 0 };
                DWORD dwOffset               = 0x00;
                PCHAR FuncMod                = NULL;
                PCHAR nwFuncName             = NULL;

                MemCopy( ForwarderName, pFunctionAddress, StringLengthA((PCHAR)pFunctionAddress) );

                for (int i = 0; i < StringLengthA( (PCHAR)ForwarderName ); i++) {
                    if (((PCHAR)ForwarderName)[i] == '.') {
                        dwOffset = i;
                        ForwarderName[i] = NULL;
                        break;
                    }
                }

                FuncMod = ForwarderName;
                nwFuncName = ForwarderName + dwOffset + 1;

                return C_PTR(pFunctionAddress);
            }
        }
    }

    return NULL;
}
