#include "math.h"

Matrix mtxIdentity()
{
    return { 1.f, 0.f, 0.f, 0.f,
             0.f, 1.f, 0.f, 0.f,
             0.f, 0.f, 1.f, 0.f,
             0.f, 0.f, 0.f, 1.f };
}

Matrix mtxLookAt(const Vec3& eye, const Vec3& at, const Vec3& up)
{
    Vec3 atCopy = at;

    const Vec3 NormAxisZ = Normalize(atCopy - Eye);
    const Vec3 NormAxisX = Normalize(Cross(Up, NormAxisZ));
    const Vec3 NormAxisY = Cross(NormAxisZ, NormAxisX);

    return FMatrix(NormAxisX.X, NormAxisY.X, NormAxisZ.X, 0.f,
                   NormAxisX.Y, NormAxisY.Y, NormAxisZ.Y, 0.f,
                   NormAxisX.Z, NormAxisY.Z, NormAxisZ.Z, 0.f,
                   -FMath::Dot(NormAxisX, Eye), -FMath::Dot(NormAxisY, Eye), -FMath::Dot(NormAxisZ, Eye), 1.f);
}
