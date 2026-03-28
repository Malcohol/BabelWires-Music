#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #ifdef SMFLIB_EXPORTS
        #define SMFLIB_API __declspec(dllexport)
    #else
        #define SMFLIB_API __declspec(dllimport)
    #endif
#else
    #define SMFLIB_API __attribute__((visibility("default")))
#endif