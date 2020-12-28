#ifndef VECTOR_H
#define VECTOR_H

struct Vec2
{
public:
    float X, Y;

public:

    //////////////////////////////////////
    /// Vector2(default)

    inline Vec2()
            : X(0.0f)
            , Y(0.0f)
    {}

    //////////////////////////////////////
    /// Vector2(copy constructor)

    inline Vec2(const Vec2& copy)
    {
        *this = copy;
    }

    //////////////////////////////////////
    /// Vector2(set constructor)

    inline Vec2(const float x, const float y)
    {
        this->Set(x, y);
    }

    //////////////////////////////////////
    /// Vector2(scalar constructor)

    inline Vec2(const float scalar)
    {
        this->Set(scalar, scalar);
    }

    //////////////////////////////////////
    /// Set

    inline void Set(const float x, const float y)
    {
        X = x;
        Y = y;
    }

    //////////////////////////////////////
    /// operator=

    inline Vec2& operator=(const Vec2& rhe)
    {
        Set(rhe.X, rhe.Y);
        return *this;
    }

    //////////////////////////////////////
    /// operator+

    inline const Vec2 operator+(const Vec2& rhe) const
    {
        return Vec2(X + rhe.X, Y + rhe.Y);
    }

    //////////////////////////////////////
    /// operator+=

    inline Vec2& operator+=(const Vec2& rhe)
    {
        X += rhe.X;
        Y += rhe.Y;

        return *this;
    }

    //////////////////////////////////////
    /// operator-

    inline const Vec2 operator-(const Vec2& rhe) const
    {
        return Vec2(X - rhe.X, Y - rhe.Y);
    }

    //////////////////////////////////////
    /// operator-=

    inline Vec2& operator-=(const Vec2& rhe)
    {
        X -= rhe.X;
        Y -= rhe.Y;

        return *this;
    }

    //////////////////////////////////////
    /// operator*(vector)

    inline const Vec2 operator*(const Vec2& rhe) const
    {
        return Vec2(X * rhe.X, Y * rhe.Y);
    }

    //////////////////////////////////////
    /// operator*=(vector)

    inline Vec2& operator*=(const Vec2& rhe)
    {
        X *= rhe.X;
        Y *= rhe.Y;

        return *this;
    }

    //////////////////////////////////////
    /// operator*(scalar)

    inline const Vec2 operator*(const float scalar) const
    {
        return Vec2(X * scalar, Y * scalar);
    }

    //////////////////////////////////////
    /// operator*=(scalar)

    inline Vec2& operator*=(const float scalar)
    {
        X *= scalar;
        Y *= scalar;

        return *this;
    }

    //////////////////////////////////////
    /// operator/(vector)

    inline const Vec2 operator/(const Vec2& rhe) const
    {
        return Vec2(X / rhe.X, Y / rhe.Y);
    }

    //////////////////////////////////////
    /// operator/=(vector)

    inline Vec2& operator/=(const Vec2& rhe)
    {
        X /= rhe.X;
        Y /= rhe.Y;

        return *this;
    }

    //////////////////////////////////////
    /// operator/(scalar)

    inline const Vec2 operator/(const float scalar) const
    {
        return Vec2(X / scalar, Y / scalar);
    }

    //////////////////////////////////////
    /// operator/=(scalar)

    inline Vec2& operator/=(const float scalar)
    {
        X /= scalar;
        Y /= scalar;

        return *this;
    }

    //////////////////////////////////////
    /// operator==

    inline bool operator==(const Vec2& rhe) const
    {
        return X == rhe.X && Y == rhe.Y;
    }

    //////////////////////////////////////
    /// operator!=

    inline bool operator!=(const Vec2& rhe) const
    {
        return !(*this == rhe);
    }
};


struct Vec3
{
public:
    float X, Y, Z;

public:

    //////////////////////////////////////
    /// Vector3(default)

    inline Vec3()
            : X(0.0f)
            , Y(0.0f)
            , Z(0.0f)
    {}

    //////////////////////////////////////
    /// Vector3(copy constructor)

    inline Vec3(const Vec3& copy)
    {
        *this = copy;
    }

    //////////////////////////////////////
    /// Vector3(set constructor)

    inline Vec3(const float x, const float y, const float z)
    {
        Set(x, y, z);
    }

    //////////////////////////////////////
    /// Vector3(scalar constructor)

    inline Vec3(const float scalar)
    {
        Set(scalar, scalar, scalar);
    }

    //////////////////////////////////////
    /// Set

    inline void Set(const float x, const float y, const float z)
    {
        X = x;
        Y = y;
        Z = z;
    }

    //////////////////////////////////////
    /// operator=

    inline Vec3& operator=(const Vec3& rhe)
    {
        Set(rhe.X, rhe.Y, rhe.Z);
        return *this;
    }

    //////////////////////////////////////
    /// operator+

    inline const Vec3 operator+(const Vec3& rhe) const
    {
        return Vec3(X + rhe.X, Y + rhe.Y, Z + rhe.Z);
    }

    //////////////////////////////////////
    /// operator+=

    inline Vec3& operator+=(const Vec3& rhe)
    {
        X += rhe.X;
        Y += rhe.Y;
        Z += rhe.Z;

        return *this;
    }

    //////////////////////////////////////
    /// operator-

    inline const Vec3 operator-(const Vec3& rhe) const
    {
        return Vec3(X - rhe.X, Y - rhe.Y, Z - rhe.Z);
    }

    //////////////////////////////////////
    /// operator-=

    inline Vec3& operator-=(const Vec3& rhe)
    {
        X -= rhe.X;
        Y -= rhe.Y;
        Z -= rhe.Z;

        return *this;
    }

    //////////////////////////////////////
    /// operator*(vector)

    inline const Vec3 operator*(const Vec3& rhe) const
    {
        return Vec3(X * rhe.X, Y * rhe.Y, Z * rhe.Z);
    }

    //////////////////////////////////////
    /// operator*=(vector)

    inline Vec3& operator*=(const Vec3& rhe)
    {
        X *= rhe.X;
        Y *= rhe.Y;
        Z *= rhe.Z;

        return *this;
    }

    //////////////////////////////////////
    /// operator*(scalar)

    inline const Vec3 operator*(const float scalar) const
    {
        return Vec3(X * scalar, Y * scalar, Z * scalar);
    }

    //////////////////////////////////////
    /// operator*=(scalar)

    inline Vec3& operator*=(const float scalar)
    {
        X *= scalar;
        Y *= scalar;
        Z *= scalar;

        return *this;
    }

    //////////////////////////////////////
    /// operator/(vector)

    inline const Vec3 operator/(const Vec3& rhe) const
    {
        return Vec3(X / rhe.X, Y / rhe.Y, Z / rhe.Z);
    }

    //////////////////////////////////////
    /// operator/=(vector)

    inline Vec3& operator/=(const Vec3& rhe)
    {
        X /= rhe.X;
        Y /= rhe.Y;
        Z /= rhe.Z;

        return *this;
    }

    //////////////////////////////////////
    /// operator/(scalar)

    inline const Vec3 operator/(const float scalar) const
    {
        return Vec3(X / scalar, Y / scalar, Z / scalar);
    }

    //////////////////////////////////////
    /// operator/=(scalar)

    inline Vec3& operator/=(const float scalar)
    {
        X /= scalar;
        Y /= scalar;
        Z /= scalar;

        return *this;
    }

    //////////////////////////////////////
    /// operator==

    inline bool operator==(const Vec3& rhe) const
    {
        return X == rhe.X && Y == rhe.Y && Z == rhe.Z;
    }

    //////////////////////////////////////
    /// operator!=

    inline bool operator!=(const Vec3& rhe) const
    {
        return !(*this == rhe);
    }
};

#endif // VECTOR_H