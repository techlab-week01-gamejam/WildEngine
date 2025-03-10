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
    vsprintf_s(buffer, format, args); // 가변인자 처리
    va_end(args);

    // 문자열 끝에 줄바꿈(\n) 자동 추가
    FString output(buffer);
    output += "\n";

    OutputDebugStringA(output.c_str()); // ANSI 문자열 출력
}
