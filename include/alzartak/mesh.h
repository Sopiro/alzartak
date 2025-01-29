#pragma once

#include "common.h"
#include "util.h"

namespace alzartak
{

struct Vertex
{
    Point3 position;
    Vec3 normal;
    Vec3 tangent;
    Point2 texcoord;
};

class Mesh : NonCopyable
{
public:
    Mesh(std::span<Vertex> vertices, std::span<int32> indices);
    ~Mesh() noexcept;

    void Draw(GLenum draw_mode = GL_TRIANGLES) const;

    const size_t num_vertices, num_indices;

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};

} // namespace alzartak