#pragma once

#include "common.h"

namespace alzartak
{

struct Camera
{
    Point3 position{ 0 };
    Point3 rotation{ 0 };
    Point3 scale{ 1 };

    Mat4 GetCameraMatrix() const
    {
        // Inverse scale
        Mat4 m{ Vec4(1 / scale, 1) };

        // Inverse rotation
        m = MulT(Mat4(Quat::FromEuler(rotation), Vec3::zero), m);

        // Inverse translation
        m = m.Translate(-position);

        return m;
    }
};

} // namespace alzartak