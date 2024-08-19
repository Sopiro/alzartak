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

Vec3 rgb2hsl(float r, float g, float b);
Vec3 hsl2rgb(float h, float s, float l);

} // namespace alzartak