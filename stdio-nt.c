#include <Veil\Veil.h>
#include <stdio.h>

int main(void)
{
    wchar_t wString[BUFSIZ];

    IO_STATUS_BLOCK IoStatusBlock;
    HANDLE hStdIn = NtCurrentPeb()->ProcessParameters->StandardInput;
    NtReadFile(hStdIn,
                NULL,
                NULL,
                NULL,
                &IoStatusBlock,
                (PVOID)wString,
                sizeof(wString),
                NULL,
                NULL);
    HANDLE hStdOut = NtCurrentPeb()->ProcessParameters->StandardOutput;
    NtWriteFile(hStdOut,
                NULL,
                NULL,
                NULL,
                &IoStatusBlock,
                (PVOID)wString,
                IoStatusBlock.Information,
                NULL,
                NULL);
    RtlExitUserProcess(STATUS_SUCCESS);
}
