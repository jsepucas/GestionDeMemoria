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

