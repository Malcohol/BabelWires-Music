#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #ifdef SEQ2TAPELIB_EXPORTS
        #define SEQ2TAPELIB_API __declspec(dllexport)
    #else
        #define SEQ2TAPELIB_API __declspec(dllimport)
    #endif
#else
    #define SEQ2TAPELIB_API __attribute__((visibility("default")))
#endif