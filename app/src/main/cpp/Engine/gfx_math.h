#ifndef MATH_H
#define MATH_H

#include "matrix.h"
#include "vector.h"

#include <cstdint>
#include <cmath>

constexpr const float PI = 3.14159265359f;

typedef struct {
    float Left;
    float Right;
    float Bottom;
    float Top;
} ScreenRect;

inline float Magnitude(const Vec2& input)
{
    return sqrtf((input.X * input.X) + (input.Y * input.Y));
}

inline float Magnitude(const Vec3& input)
{
    return sqrtf((input.X * input.X) + (input.Y * input.Y) + (input.Z * input.Z));
}

inline Vec2 Normalize(const Vec2& input)
{
    const float length = Magnitude(input);
    return { input.X / length, input.Y / length };
}

inline Vec3 Normalize(const Vec3& input)
{
    const float length = Magnitude(input);
    return { input.X / length, input.Y / length, input.Z / length };
}

inline Vec3 Cross(const Vec3& lhe, const Vec3& rhe)
{
    return { lhe.Y * rhe.Z - lhe.Z * rhe.Y,
             lhe.Z * rhe.X - lhe.X * rhe.Z,
             lhe.X * rhe.Y - lhe.Y * rhe.X };
}

inline float Dot(const Vec2& lhe, const Vec2& rhe)
{
    return lhe.X * rhe.X + lhe.Y * rhe.Y;
}

inline float Dot(const Vec3& lhe, const Vec3& rhe)
{
    return lhe.X * rhe.X + lhe.Y * rhe.Y + lhe.Z * rhe.Z;
}

inline float Abs(const float& input)
{
    return fabsf(input);
}

inline float Cos(const float& input)
{
    return cosf(input);
}

inline float Sin(const float& input)
{
    return sinf(input);
}

inline float Tan(const float& input)
{
    return tanf(input);
}

inline float ToRadians(const float& input)
{
    return (PI / 180.0f) * input;
}

inline float ToDegrees(const float& input)
{
    return (180.0f / PI) * input;
}

inline Matrix mtxIdentity()
{
    return { 1.0f, 0.0f, 0.0f, 0.0f,
             0.0f, 1.0f, 0.0f, 0.0f,
             0.0f, 0.0f, 1.0f, 0.0f,
             0.0f, 0.0f, 0.0f, 1.0f };
}

inline Matrix mtxLookAt(const Vec3& eye, const Vec3& at, const Vec3& up)
{
    Vec3 atCopy = at;

    const Vec3 normZ = Normalize(atCopy - eye);
    const Vec3 normX = Normalize(Cross(up, normZ));
    const Vec3 normY = Cross(normZ, normX);

    return { normX.X, normY.X, normZ.X, 0.0f,
             normX.Y, normY.Y, normZ.Y, 0.0f,
             normX.Z, normY.Z, normZ.Z, 0.0f,
             -Dot(normX, eye), -Dot(normY, eye), -Dot(normZ, eye), 1.0f };
}

inline Matrix mtxOrtho(const uint32_t width, const uint32_t height, const float minZ, const float maxZ)
{
    const float range = (maxZ - minZ);
    return { 2.0f / width, 0.0f         , 0.0f         , 0.0f,
             0.0f        , 2.0f / height, 0.0f         , 0.0f,
             0.0f        , 0.0f         , 1.0f / range , 0.0f,
             0.0f        , 0.0f         , -minZ / range, 1.0f };
}

inline Matrix mtxOrthoOffCenter(const ScreenRect& rect, const float minZ, const float maxZ)
{
    const float range = (maxZ - minZ);

    const float width  = (rect.Right - rect.Left);
    const float height = (rect.Top - rect.Bottom);

    const float offsetX = (rect.Left + rect.Right) / (rect.Left - rect.Right);
    const float offsetY = (rect.Top + rect.Bottom) / (rect.Bottom - rect.Top);

    return { 2.0f / width, 0.0f         , 0.0f        , 0.0f,
             0.0f        , 2.0f / height, 0.0f        , 0.0f,
             0.0f        , 0.0f         , 1.0f / range, 0.0f,
             offsetX, offsetY, minZ / (minZ - maxZ), 1.0f };
}

inline Matrix mtxPerspective(const float fov, const float aspect, const float minZ, const float maxZ)
{
    const float verticalScale   = 1.0f / (Tan(ToRadians(fov) * 0.5f));
    const float horizontalScale = verticalScale / aspect;

    return { horizontalScale, 0.0f         , 0.0f                          , 0.0f,
             0.0f           , verticalScale, 0.0f                          , 0.0f,
             0.0f           , 0.0f         , maxZ / (maxZ - minZ)          , 1.0f,
             0.0f           , 0.0f         , (-maxZ) * minZ / (maxZ - minZ), 0.0f };
}

inline Matrix mtxTranslate(const Vec3& offset)
{
    return { 1.0f    , 0.0f    , 0.0f    , 0.0f,
             0.0f    , 1.0f    , 0.0f    , 0.0f,
             0.0f    , 0.0f    , 1.0f    , 0.0f,
             offset.X, offset.Y, offset.Z, 1.0f };
}

inline Matrix mtxScale(const Vec3& offset)
{
    return { offset.X, 0.0f    , 0.0f    , 0.0f,
             0.0f    , offset.Y, 0.0f    , 0.0f,
             0.0f    , 0.0f    , offset.Z, 0.0f,
             0.0f    , 0.0f    , 0.0f    , 1.0f };
}

inline Matrix mtxRotateX(const float angle)
{
    return { 1.0f, 0.0f      , 0.0f      , 0.0f,
             0.0f, Cos(angle), Sin(angle), 0.0f,
             0.0f,-Sin(angle), Cos(angle), 0.0f,
             0.0f, 0.0f      , 0.0f      , 1.0f };
}

inline Matrix mtxRotateY(const float angle)
{
    return { Cos(angle), 0.0f,-Sin(angle), 0.0f,
             0.0f      , 1.0f, 0.0f      , 0.0f,
             Sin(angle), 0.0f, Cos(angle), 0.0f,
             0.0f      , 0.0f, 0.0f      , 1.0f };
}

inline Matrix mtxRotateZ(const float angle)
{
    return { Cos(angle), Sin(angle), 0.0f, 0.0f,
            -Sin(angle), Cos(angle), 0.0f, 0.0f,
             0.0f      , 0.0f      , 1.0f, 0.0f,
             0.0f      , 0.0f      , 0.0f, 1.0f };
}

inline Matrix mtxTranspose(const Matrix& mtx)
{
    return { mtx.M[0][0], mtx.M[1][0], mtx.M[2][0], mtx.M[3][0],
             mtx.M[0][1], mtx.M[1][1], mtx.M[2][1], mtx.M[3][1],
             mtx.M[0][2], mtx.M[1][2], mtx.M[2][2], mtx.M[3][2],
             mtx.M[0][3], mtx.M[1][3], mtx.M[2][3], mtx.M[3][3] };
}

template <typename Type>
inline void ClampMin(Type& value, const Type min)
{
    if (value < min) {
        value = min;
    }
}

template <typename Type>
inline void ClampMax(Type& value, const Type max)
{
    if (value > max) {
        value = max;
    }
}

template <typename Type>
inline void Clamp(Type& value, const Type min, const Type max)
{
    ClampMin(value, min);
    ClampMax(value, max);
}


#endif // MATH_H