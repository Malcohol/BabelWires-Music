#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #if defined(MUSICLIBUI_STATIC)
        #define MUSICLIBUI_API
    #elif defined(MUSICLIBUI_EXPORTS)
        #define MUSICLIBUI_API __declspec(dllexport)
    #else
        #define MUSICLIBUI_API __declspec(dllimport)
    #endif
#else
    #define MUSICLIBUI_API __attribute__((visibility("default")))
#endif