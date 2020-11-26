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

#endif // UTILS_H