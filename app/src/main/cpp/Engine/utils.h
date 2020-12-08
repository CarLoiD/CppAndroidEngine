#ifndef UTILS_H
#define UTILS_H

#include <jni.h>
#include <android/log.h>

#define EVALUATE_LOGS 1

#if EVALUATE_LOGS == 1
#define LogDebug(...) __android_log_print(ANDROID_LOG_DEBUG, "LOGDEBUG", __VA_ARGS__)
#define LogError(...) __android_log_print(ANDROID_LOG_ERROR, "LOGERROR", __VA_ARGS__)
#else
#define LogDebug(...)
#define LogError(...)
#endif

inline void DwordToColor(const uint32_t dword, float& r, float& g, float& b, float& a)
{
    r = (float)((dword & 0xFF000000) >> 24);
    g = (float)((dword & 0x00FF0000) >> 16);
    b = (float)((dword & 0x0000FF00) >> 8);
    a = (float)((dword & 0x000000FF));
}

inline void DwordToColorNormalized(const uint32_t dword, float& r, float& g, float& b, float& a)
{
    DwordToColor(dword, r, g, b, a);
    r /= 255.0f;
    g /= 255.0f;
    b /= 255.0f;
    a /= 255.0f;
}

#endif // UTILS_H