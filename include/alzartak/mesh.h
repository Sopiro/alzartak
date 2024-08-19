#pragma once

#include "common.h"
#include "util.h"

namespace alzartak
{

class Mesh : NonCopyable
{
public:
    Mesh(std::span<Vec3> vertices, std::span<Vec2> tex_coords, std::span<int32> indices);
    ~Mesh() noexcept;

    void Draw(GLenum draw_mode = GL_TRIANGLES) const;

private:
    std::vector<Vec3> vertices;
    std::vector<Vec2> tex_coords;
    std::vector<int32> indices;

    GLuint VAO;
    GLuint VBOv;
    GLuint VBOt;
    GLuint EBOt;
    GLuint EBOl;
};

} // namespace alzartak