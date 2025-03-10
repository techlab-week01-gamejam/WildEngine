#include "DebugConsole.h"
#include <windows.h>
#include <cstdarg>
#include <cstdio>
#include "Types/Types.h"

FDebugConsole::FDebugConsole()
{
}

void FDebugConsole::DebugPrint(const char* format, ...)
{
    char buffer[512]; // 출력 버퍼
    va_list args;
    va_start(args, format);
    vsprintf_s(buffer, format, args); // 가변?�자 처리
    va_end(args);

    // 문자???�에 줄바�?\n) ?�동 추�?
    FString output(buffer);
    output += "\n";

    OutputDebugStringA(output.c_str()); // ANSI 문자??출력
}
