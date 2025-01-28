#pragma once

#include "common.h"

namespace alzartak
{

class Texture
{
public:
    Texture(const std::string& filepath);
    ~Texture();

    void Bind(uint32 unit = 0) const;
    void Unbind() const;

    GLuint GetID() const
    {
        return id;
    }

    int32 GetWidth() const
    {
        return width;
    }

    int32 GetHeight() const
    {
        return height;
    }

private:
    GLuint id;
    int32 width, height, channels;
};

} // namespace alzartak