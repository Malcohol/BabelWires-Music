#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #if defined(TESTPLUGINLIB_STATIC)
        #define TESTPLUGINLIB_API
    #elif defined(TESTPLUGINLIB_EXPORTS)
        #define TESTPLUGINLIB_API __declspec(dllexport)
    #else
        #define TESTPLUGINLIB_API __declspec(dllimport)
    #endif
#else
    #define TESTPLUGINLIB_API __attribute__((visibility("default")))
#endif