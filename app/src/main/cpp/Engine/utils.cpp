#include "utils.h"

#include <android/log.h>

void LogDebug(const char* tag, const char* message)
{
    __android_log_print(ANDROID_LOG_DEBUG, tag, "%s", message);
}

void LogError(const char* tag, const char* message)
{
    __android_log_print(ANDROID_LOG_ERROR, tag, "%s", message);
}
