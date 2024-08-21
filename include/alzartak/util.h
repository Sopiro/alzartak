#pragma once

#include "common.h"

namespace alzartak
{

class NonCopyable
{
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    void operator=(const NonCopyable&) = delete;
};

Vec3 RGBtoHSL(Vec3 rgb);
Vec3 HSLtoRGB(Vec3 hsl);

} // namespace alzartak