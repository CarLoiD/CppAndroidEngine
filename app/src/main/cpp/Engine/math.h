#ifndef MATH_H
#define MATH_H

#include "matrix.h"
#include "vector.h"

#include <cstdint>

/// MATRIX
Matrix mtxIdentity();
Matrix mtxLookAt(const Vec3& eye, const Vec3& at, const Vec3& up);
Matrix mtxOrtho(const uint32_t width, const uint32_t height, const float minZ, const float maxZ);
Matrix mtxOrthoOffCenter(const uint32_t left, const uint32_t right, const uint32_t bottom, const uint32_t up, const float minZ, const float maxZ);
Matrix mtxPerspective(const float fov, const float aspect, const float minZ, const float maxZ);
Matrix mtxTranslate(const Vec3& offset);
Matrix mtxScale(const Vec3& offset);
Matrix mtxRotateX(const float angle);
Matrix mtxRotateY(const float angle);
Matrix mtxRotateZ(const float angle);
Matrix mtxTranspose(const Matrix& mtx);

/// VECTOR
Vec2 vec2Cross(const Vec2& lhe, const Vec2& rhe);
Vec3 vec3Cross(const Vec3& lhe, const Vec3& rhe);

/// SCALAR
float vec2Dot(const Vec2& lhe, const Vec2& rhe);
float vec3Dot(const Vec3& lhe, const Vec3& rhe);

#endif // MATH_H