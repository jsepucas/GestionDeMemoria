#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <process.h>


#define SIZE 4096


unsigned int __stdcall ChildThread(void* pBuf);

int main() {
    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,   // usar archivo de paginación
            nullptr,                   // seguridad predeterminada
            PAGE_READWRITE,         // acceso de lectura/escritura
            0,                      // tamaño máximo del objeto (DWORD de orden superior)
            SIZE,                   // tamaño máximo del objeto (DWORD de orden inferior)
            _T("Local\\MyFileMappingObject")); // nombre del objeto de mapeo de archivos

    if (hMapFile == nullptr) {
        _tprintf(_T("No se  ha podido crear el objeto de mapeo de archivos (%lu).\n"),    GetLastError());
        return 1;
    }

    pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // manejador del objeto de mapeo
                                 FILE_MAP_ALL_ACCESS, // permiso de lectura/escritura
                                 0,
                                 0,
                                 SIZE);

    if (pBuf == nullptr) {
        _tprintf(_T("No se ha podido mapear la vista del archivo (%lu).\n"), GetLastError());

        CloseHandle(hMapFile);
        return 1;
    }


    auto hThread = (HANDLE)_beginthreadex(nullptr, 0, ChildThread, (void*)pBuf, 0, nullptr);
    if (hThread == nullptr) {
        _tprintf(_T("No se ha podido crear  hilo secundario.\n"));

        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        return 1;
    }


    _tcscpy_s((TCHAR*)pBuf, SIZE, _T("Hello, child process!"));


    WaitForSingleObject(hThread, INFINITE);


    CloseHandle(hThread);

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}


unsigned int __stdcall ChildThread(void* pBuf) {
    _tprintf(_T("Child reads: %s\n"), (TCHAR*)pBuf);
    return 0;

}