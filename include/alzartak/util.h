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

} // namespace alzartak