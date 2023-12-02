#include <Veil\\Veil.h>
#include <stdio.h>
#include <stdlib.h>

int wmain(int argc, wchar_t **argv)
{
    UNICODE_STRING NtPathU;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;
    HANDLE FileHandle;
    NTSTATUS status;

    if(argc == 1) {
        fputs("Usage : cat %s\n", stderr);
        exit(EXIT_FAILURE);
    }

    RtlDosPathNameToNtPathName_U(argv[1], &NtPathU, NULL, NULL);

    InitializeObjectAttributes(&ObjectAttributes, &NtPathU, OBJ_CASE_INSENSITIVE, NULL, NULL);
    status = NtCreateFile(&FileHandle,
                          FILE_GENERIC_READ,
                          &ObjectAttributes,
                          &IoStatusBlock,
                          NULL,
                          FILE_ATTRIBUTE_NORMAL,
                          FILE_SHARE_READ,
                          FILE_OPEN,
                          FILE_RANDOM_ACCESS | FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
                          NULL,
                          0);

    if (status == STATUS_SUCCESS) {
        _putws(L"File opened successfully!");
        
        FILE_STANDARD_INFORMATION FileStandard;
        NtQueryInformationFile(FileHandle,
                               &IoStatusBlock,
                               &FileStandard,
                               sizeof(FILE_STANDARD_INFORMATION),
                               FileStandardInformation);

        DWORD dwFileSize = FileStandard.EndOfFile.u.LowPart;
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
        
        NtReadFile(FileHandle,
                   NULL,
                   NULL,
                   NULL,
                   &IoStatusBlock,
                   (PVOID)buf,
                   dwFileSize,
                   NULL,
                   NULL);

        HANDLE hStdOut = NtCurrentPeb()->ProcessParameters->StandardOutput;
        NtWriteFile(hStdOut,
                    NULL,
                    NULL,
                    NULL,
                    &IoStatusBlock,
                    (PVOID)buf,
                    IoStatusBlock.Information,
                    NULL,
                    NULL);

        free(buf);
        NtClose(FileHandle);
    } else {
        fprintf(stderr, "Failed to open file. Error code: 0x%lx\n", status);
    }

    RtlExitUserProcess(status);
}
