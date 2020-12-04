#include "touchscreen.h"

void TouchScreen::Create(const uint32_t* jniEnvPtr, const uint32_t width, const uint32_t height)
{
    mJniEnv = (JNIEnv*)jniEnvPtr;

    mWidth  = width;
    mHeight = height;
}

float TouchScreen::GetTouchScreenX(const TouchScreenId& id) const
{
    mClass = mJniEnv->FindClass("com/carloid/cppandroidengine/MainActivity");

    const float xAxisOutput = [&]()
    {
        switch (id)
        {
        case TouchScreenId::Touch:
            mCursorAxisXId = mJniEnv->GetStaticFieldID(mClass, "mCursorAxisX", "F");
            return mJniEnv->GetStaticFloatField(mClass, mCursorAxisXId);

        case TouchScreenId::MultiTouch:
            mMultiCursorAxisXId = mJniEnv->GetStaticFieldID(mClass, "mMultiCursorAxisX", "F");
            return mJniEnv->GetStaticFloatField(mClass, mMultiCursorAxisXId);
        }
    }();

    return xAxisOutput < 0.0f ? xAxisOutput : xAxisOutput / mWidth;
}

float TouchScreen::GetTouchScreenY(const TouchScreenId& id) const
{
    mClass = mJniEnv->FindClass("com/carloid/cppandroidengine/MainActivity");

    const float yAxisOutput = [&]()
    {
        switch (id)
        {
        case TouchScreenId::Touch:
            mCursorAxisYId = mJniEnv->GetStaticFieldID(mClass, "mCursorAxisY", "F");
            return mJniEnv->GetStaticFloatField(mClass, mCursorAxisYId);

        case TouchScreenId::MultiTouch:
            mMultiCursorAxisYId = mJniEnv->GetStaticFieldID(mClass, "mMultiCursorAxisY", "F");
            return mJniEnv->GetStaticFloatField(mClass, mMultiCursorAxisYId);
        }
    }();

    return yAxisOutput < 0.0f ? yAxisOutput : yAxisOutput / mHeight;
}

Vec2 TouchScreen::GetTouchScreenXY(const TouchScreenId& id) const
{
    return { GetTouchScreenX(id), GetTouchScreenY(id) };
}