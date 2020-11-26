#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <jni.h>
#include <cstdint>

#include "vector.h"

typedef enum class TOUCHSCREEN_ID {
    Touch,
    MultiTouch
} TouchScreenId;

class TouchScreen final
{
public:
    void Create(const uint32_t* jniEnvPtr, const uint32_t width, const uint32_t height);

    float GetTouchScreenX(const TouchScreenId& id) const;
    float GetTouchScreenY(const TouchScreenId& id) const;

    Vec2 GetTouchScreenXY(const TouchScreenId& id) const;

private:
    JNIEnv* mJniEnv;
    mutable jclass mClass;

    uint32_t mWidth;
    uint32_t mHeight;

    mutable jfieldID mCursorAxisXId;
    mutable jfieldID mCursorAxisYId;

    // MultiTouch id's
    mutable jfieldID mMultiCursorAxisXId;
    mutable jfieldID mMultiCursorAxisYId;
};

#endif // TOUCHSCREEN_H