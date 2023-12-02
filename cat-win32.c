#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int wmain(int argc, wchar_t **argv)
{
    if (argc == 1) {
        fputs("Usage : cat %s\n", stderr);
        exit(EXIT_FAILURE);
    }

    HANDLE hFile = CreateFileW(argv[1],
                               GENERIC_READ,
                               FILE_SHARE_READ,
                               NULL,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        _putws(L"File opened successfully");
        DWORD dwFileSize = GetFileSize(hFile, NULL);
        static BYTE *buf = NULL, mem[BUFSIZ];
        if (sizeof(mem) < dwFileSize) {
            buf = malloc(dwFileSize * sizeof(BYTE));
            if (buf == NULL) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
        } else {
            buf = mem;
        }

        DWORD dwReaded;
        ReadFile(hFile, buf, dwFileSize, &dwReaded, NULL);
        WriteFile(ULongToHandle(STD_OUTPUT_HANDLE), buf, dwReaded, NULL, NULL);
        CloseHandle(hFile);
    } else {
        fprintf(stderr, "Failed to open file. Error code: 0x%X", (unsigned int)GetLastError());
    }

    ExitProcess(GetLastError());
}
