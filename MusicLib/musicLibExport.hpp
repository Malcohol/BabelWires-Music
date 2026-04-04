#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #if defined(MUSICLIB_STATIC)
        #define MUSICLIB_API
    #elif defined(MUSICLIB_EXPORTS)
        #define MUSICLIB_API __declspec(dllexport)
    #else
        #define MUSICLIB_API __declspec(dllimport)
    #endif
#else
    #define MUSICLIB_API __attribute__((visibility("default")))
#endif