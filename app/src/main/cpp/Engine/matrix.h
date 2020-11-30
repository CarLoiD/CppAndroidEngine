#ifndef MATRIX_H
#define MATRIX_H

struct Matrix {
public:
    float M[4][4];

public:
    Matrix() = default;

    Matrix(const float m00, const float m01, const float m02, const float m03,
           const float m10, const float m11, const float m12, const float m13,
           const float m20, const float m21, const float m22, const float m23,
           const float m30, const float m31, const float m32, const float m33)
    {
        Set(m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33);
    }

    inline void Set(const float m00, const float m01, const float m02, const float m03,
                    const float m10, const float m11, const float m12, const float m13,
                    const float m20, const float m21, const float m22, const float m23,
                    const float m30, const float m31, const float m32, const float m33)
    {
        M[0][0] = m00; M[0][1] = m01; M[0][2] = m02; M[0][3] = m03;
        M[1][0] = m10; M[1][1] = m11; M[1][2] = m12; M[1][3] = m13;
        M[2][0] = m20; M[2][1] = m21; M[2][2] = m22; M[2][3] = m23;
        M[3][0] = m30; M[3][1] = m31; M[3][2] = m32; M[3][3] = m33;
    }

    inline Matrix& operator*(const Matrix& rhe)
    {
        Set(M[0][0] * rhe.M[0][0] + M[0][1] * rhe.M[1][0] + M[0][2] * rhe.M[2][0] + M[0][3] * rhe.M[3][0],
            M[0][0] * rhe.M[0][1] + M[0][1] * rhe.M[1][1] + M[0][2] * rhe.M[2][1] + M[0][3] * rhe.M[3][1],
            M[0][0] * rhe.M[0][2] + M[0][1] * rhe.M[1][2] + M[0][2] * rhe.M[2][2] + M[0][3] * rhe.M[3][2],
            M[0][0] * rhe.M[0][3] + M[0][1] * rhe.M[1][3] + M[0][2] * rhe.M[2][3] + M[0][3] * rhe.M[3][3],

            M[1][0] * rhe.M[0][0] + M[1][1] * rhe.M[1][0] + M[1][2] * rhe.M[2][0] + M[1][3] * rhe.M[3][0],
            M[1][0] * rhe.M[0][1] + M[1][1] * rhe.M[1][1] + M[1][2] * rhe.M[2][1] + M[1][3] * rhe.M[3][1],
            M[1][0] * rhe.M[0][2] + M[1][1] * rhe.M[1][2] + M[1][2] * rhe.M[2][2] + M[1][3] * rhe.M[3][2],
            M[1][0] * rhe.M[0][3] + M[1][1] * rhe.M[1][3] + M[1][2] * rhe.M[2][3] + M[1][3] * rhe.M[3][3],

            M[2][0] * rhe.M[0][0] + M[2][1] * rhe.M[1][0] + M[2][2] * rhe.M[2][0] + M[2][3] * rhe.M[3][0],
            M[2][0] * rhe.M[0][1] + M[2][1] * rhe.M[1][1] + M[2][2] * rhe.M[2][1] + M[2][3] * rhe.M[3][1],
            M[2][0] * rhe.M[0][2] + M[2][1] * rhe.M[1][2] + M[2][2] * rhe.M[2][2] + M[2][3] * rhe.M[3][2],
            M[2][0] * rhe.M[0][3] + M[2][1] * rhe.M[1][3] + M[2][2] * rhe.M[2][3] + M[2][3] * rhe.M[3][3],

            M[3][0] * rhe.M[0][0] + M[3][1] * rhe.M[1][0] + M[3][2] * rhe.M[2][0] + M[3][3] * rhe.M[3][0],
            M[3][0] * rhe.M[0][1] + M[3][1] * rhe.M[1][1] + M[3][2] * rhe.M[2][1] + M[3][3] * rhe.M[3][1],
            M[3][0] * rhe.M[0][2] + M[3][1] * rhe.M[1][2] + M[3][2] * rhe.M[2][2] + M[3][3] * rhe.M[3][2],
            M[3][0] * rhe.M[0][3] + M[3][1] * rhe.M[1][3] + M[3][2] * rhe.M[2][3] + M[3][3] * rhe.M[3][3]);

        return *this;
    }

    inline Matrix& operator*(const float scalar)
    {
        M[0][0] *= scalar; M[0][1] *= scalar; M[0][2] *= scalar; M[0][3] *= scalar;
        M[1][0] *= scalar; M[1][1] *= scalar; M[1][2] *= scalar; M[1][3] *= scalar;
        M[2][0] *= scalar; M[2][1] *= scalar; M[2][2] *= scalar; M[2][3] *= scalar;
        M[3][0] *= scalar; M[3][1] *= scalar; M[3][2] *= scalar; M[3][3] *= scalar;

        return *this;
    }
};

#endif // MATRIX_H