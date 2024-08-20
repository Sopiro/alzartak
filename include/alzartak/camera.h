#pragma once

#include "common.h"

namespace alzartak
{

struct Camera2D
{
    Point2 position{ 0 };
    float rotation{ 0 };
    Point2 scale{ 1 };

    Mat4 GetCameraMatrix() const
    {
        // Inverse scale
        Mat4 m{ Vec4(1 / scale.x, 1 / scale.y, 1, 1) };

        // Inverse rotation
        m = MulT(Mat4(Quat::FromEuler({ 0, 0, rotation }), Vec3::zero), m);

        // Inverse translation
        m = m.Translate({ -position.x, -position.y, 0 });

        return m;
    }
};

struct Camera3D
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