#include <windows.h>
#include <stdio.h>

// change color of text

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &ConsoleInfo);
    int originalAttrs = ConsoleInfo.wAttributes;

    printf("This text is ");
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
    printf("red ");
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("green ");
    SetConsoleTextAttribute(hConsole, originalAttrs);
    printf("normal\n");

    return 0;
}
