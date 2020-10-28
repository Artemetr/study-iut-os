#include "windows.h"
#include <iostream>
#include <clocale>

using namespace std;

int main() {
    STARTUPINFO cif;
    ZeroMemory(&cif,sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;

    if (CreateProcess("C:\\Users\\artemetr\\Desktop\\os-lw-2\\os-lw-2-1.exe", NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi)) {
        printf("Program has been runned\n");
        Sleep(10000);

        TerminateProcess(pi.hProcess, NO_ERROR);
        printf("Program has been stopped\n");

        system("pause");
        return EXIT_SUCCESS;
    }

    printf("Terminated Incorrectly\n");
    system("pause");
    return EXIT_FAILURE;
}