#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #ifdef MUSICLIBUI_EXPORTS
        #define MUSICLIBUI_API __declspec(dllexport)
    #else
        #define MUSICLIBUI_API __declspec(dllimport)
    #endif
#else
    #define MUSICLIBUI_API __attribute__((visibility("default")))
#endif