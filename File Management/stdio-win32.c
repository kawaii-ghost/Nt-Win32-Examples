#include <windows.h>
#include <stdio.h>

int wmain(void)
{
    wchar_t wString[BUFSIZ];
    
    DWORD dwReaded;

    HANDLE hStdIn = ULongToHandle(STD_INPUT_HANDLE);
    ReadFile(hStdIn, wString, sizeof(wString), &dwReaded, NULL);

    HANDLE hStdOut = ULongToHandle(STD_OUTPUT_HANDLE);
    WriteFile(hStdOut, wString, dwReaded, NULL, NULL);

    ExitProcess(EXIT_SUCCESS);
}
