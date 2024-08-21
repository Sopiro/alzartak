#include "alzartak/renderer.h"
#include "alzartak/util.h"

namespace alzartak
{

Renderer::Renderer()
    : point_count{ 0 }
    , line_count{ 0 }
    , triangle_count{ 0 }
{
    shader = BatchShader::Create();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &pVBO);
    glGenBuffers(1, &cVBO);

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, pVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * max_vertex_count, nullptr, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, cVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec4) * max_vertex_count, nullptr, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0);
        glEnableVertexAttribArray(1);
    }
    glBindVertexArray(0);

    if (initialized == false)
    {
        constexpr float stride = 360.0f / color_count;
        for (int32 i = 0; i < color_count; ++i)
        {
            Vec3 rgb = HSLtoRGB({ i * stride / 360.0f, 1.0f, 0.75f });

            colors[i].Set(rgb.x, rgb.y, rgb.z, 0.85f);
        }

        initialized = true;
    }
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &pVBO);
    glDeleteBuffers(1, &cVBO);
}

void Renderer::FlushPoints()
{
    shader->Use();

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, pVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vec3) * point_count, points.data());

        glBindBuffer(GL_ARRAY_BUFFER, cVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vec4) * point_count, point_colors.data());

        glDrawArrays(GL_POINTS, 0, point_count);
    }
    glBindVertexArray(0);

    point_count = 0;
}

void Renderer::FlushLines()
{
    shader->Use();

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, pVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vec3) * line_count, lines.data());

        glBindBuffer(GL_ARRAY_BUFFER, cVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vec4) * line_count, line_colors.data());

        glDrawArrays(GL_LINES, 0, line_count);
    }
    glBindVertexArray(0);

    line_count = 0;
}

void Renderer::FlushTriangles()
{
    shader->Use();

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, pVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vec3) * triangle_count, triangles.data());

        glBindBuffer(GL_ARRAY_BUFFER, cVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vec4) * triangle_count, triangle_colors.data());

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLES, 0, triangle_count);
        glDisable(GL_BLEND);
    }
    glBindVertexArray(0);

    triangle_count = 0;
}

} // namespace alzartak