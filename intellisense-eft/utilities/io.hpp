#pragma once

#include <Windows.h>
#include <cstdint>
#include <iostream>

namespace utilities::io
{
    constexpr auto debug = true;

    inline void initiate( const char* const console_name )
    {
        if constexpr (debug)
        {
            AllocConsole();

            FILE* file_stream;

            freopen_s(&file_stream, "CONIN$", "r", stdin);
            freopen_s(&file_stream, "CONOUT$", "w", stdout);
            freopen_s(&file_stream, "CONOUT$", "w", stderr);

            SetConsoleTitleA(console_name);
        }
    }

    inline void log( const char* const format, const auto&... args )
    {
        if constexpr ( debug )
            std::printf( format, args... );
    }
}