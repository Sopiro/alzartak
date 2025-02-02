#pragma once

#include "batch_shader.h"
#include "colors.h"
#include "window.h"

namespace alzartak
{

class BatchRenderer : NonCopyable
{
public:
    struct Vertex
    {
        Point3 point;
        Vec4 color;
    };

private:
    static inline bool initialized = false;

    static constexpr inline int32 color_count = 10;
    static inline std::array<Vec4, color_count> colors;

    static constexpr inline int32 max_vertex_count = 1024 * 3;
    static_assert(max_vertex_count % 2 == 0);
    static_assert(max_vertex_count % 3 == 0);

public:
    BatchRenderer();
    ~BatchRenderer();

    void SetPointSize(float size) const;
    void SetLineWidth(float line_width) const;

    void SetProjectionMatrix(const Mat4& proj_matrix);
    void SetViewMatrix(const Mat4& view_matrix);

    void DrawPoint(const Vertex& v);
    void DrawPoint(const Point2& p, const Vec4& color = Vec4(Color::black, 0.8f));
    void DrawPoint(const Point3& p, const Vec4& color = Vec4(Color::black, 0.8f));

    void DrawLine(const Vertex& v1, const Vertex& v2);
    void DrawLine(const Point2& p1, const Point2& p2, const Vec4& color = Vec4(Color::black, 0.8f));
    void DrawLine(const Point3& p1, const Point3& p2, const Vec4& color = Vec4(Color::black, 0.8f));

    void DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
    void DrawTriangle(const Point2& p1, const Point2& p2, const Point2& p3, const Vec4& color = Vec4(Color::black, 0.8f));
    void DrawTriangle(const Point3& p1, const Point3& p2, const Point3& p3, const Vec4& color = Vec4(Color::black, 0.8f));

    void DrawAABB(const AABB2& aabb, const Vec4& color = Vec4(Color::black, 0.8f));
    void DrawAABB(const AABB3& aabb, const Vec4& color = Vec4(Color::black, 0.8f));

    void FlushAll();
    void FlushPoints();
    void FlushLines();
    void FlushTriangles();

    Point3 Pick(const Point2& screen_pos) const;

private:
    friend class BatchShader;

    std::array<Vec3, max_vertex_count> points;
    std::array<Vec4, max_vertex_count> point_colors;
    int32 point_count;

    std::array<Vec3, max_vertex_count> lines;
    std::array<Vec4, max_vertex_count> line_colors;
    int32 line_count;

    std::array<Vec3, max_vertex_count> triangles;
    std::array<Vec4, max_vertex_count> triangle_colors;
    int32 triangle_count;

    BatchShader shader;

    GLuint VAO;
    GLuint pVBO; // position buffer
    GLuint cVBO; // color buffer
};

inline void BatchRenderer::SetPointSize(float size) const
{
#ifndef __EMSCRIPTEN__
    glPointSize(size);
#else
    WakNotUsed(size);
#endif
}

inline void BatchRenderer::SetLineWidth(float line_width) const
{
    glLineWidth(line_width);
}

inline void BatchRenderer::FlushAll()
{
    if (point_count > 0) FlushPoints();
    if (line_count > 0) FlushLines();
    if (triangle_count > 0) FlushTriangles();
}

inline void BatchRenderer::SetProjectionMatrix(const Mat4& proj_matrix)
{
    shader.Use();
    shader.SetProjectionMatrix(proj_matrix);
}

inline void BatchRenderer::SetViewMatrix(const Mat4& view_matrix)
{
    shader.Use();
    shader.SetViewMatrix(view_matrix);
}

inline void BatchRenderer::DrawPoint(const Vertex& v)
{
    if (point_count == max_vertex_count)
    {
        FlushAll();
    }

    points[point_count] = v.point;
    point_colors[point_count] = v.color;
    ++point_count;
}

inline void BatchRenderer::DrawPoint(const Point2& p, const Vec4& color)
{
    DrawPoint(Point3(p, 0), color);
}

inline void BatchRenderer::DrawPoint(const Point3& p, const Vec4& color)
{
    if (point_count == max_vertex_count)
    {
        FlushAll();
    }

    points[point_count] = p;
    point_colors[point_count] = color;
    ++point_count;
}

inline void BatchRenderer::DrawLine(const Vertex& v1, const Vertex& v2)
{
    if (line_count == max_vertex_count)
    {
        FlushAll();
    }

    lines[line_count] = v1.point;
    line_colors[line_count] = v1.color;
    ++line_count;
    lines[line_count] = v2.point;
    line_colors[line_count] = v2.color;
    ++line_count;
}

inline void BatchRenderer::DrawLine(const Point2& p1, const Point2& p2, const Vec4& color)
{
    DrawLine(Point3(p1, 0), Point3(p2, 0), color);
}

inline void BatchRenderer::DrawLine(const Point3& p1, const Point3& p2, const Vec4& color)
{
    if (line_count == max_vertex_count)
    {
        FlushAll();
    }

    lines[line_count] = p1;
    line_colors[line_count] = color;
    ++line_count;
    lines[line_count] = p2;
    line_colors[line_count] = color;
    ++line_count;
}

inline void BatchRenderer::DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
    if (triangle_count == max_vertex_count)
    {
        FlushAll();
    }

    triangles[triangle_count] = v1.point;
    triangle_colors[triangle_count] = v1.color;
    ++triangle_count;
    triangles[triangle_count] = v2.point;
    triangle_colors[triangle_count] = v2.color;
    ++triangle_count;
    triangles[triangle_count] = v3.point;
    triangle_colors[triangle_count] = v3.color;
    ++triangle_count;
}

inline void BatchRenderer::DrawTriangle(const Point2& p1, const Point2& p2, const Point2& p3, const Vec4& color)
{
    DrawTriangle(Point3(p1, 0), Point3(p2, 0), Point3(p3, 0), color);
}

inline void BatchRenderer::DrawTriangle(const Point3& p1, const Point3& p2, const Point3& p3, const Vec4& color)
{
    if (triangle_count == max_vertex_count)
    {
        FlushAll();
    }

    triangles[triangle_count] = p1;
    triangle_colors[triangle_count] = color;
    ++triangle_count;
    triangles[triangle_count] = p2;
    triangle_colors[triangle_count] = color;
    ++triangle_count;
    triangles[triangle_count] = p3;
    triangle_colors[triangle_count] = color;
    ++triangle_count;
}

inline void BatchRenderer::DrawAABB(const AABB2& aabb, const Vec4& color)
{
    const Point2& min = aabb.min;
    const Point2& max = aabb.max;

    Point2 p0{ min.x, min.y };
    Point2 p1{ max.x, min.y };
    Point2 p2{ max.x, max.y };
    Point2 p3{ min.x, max.y };

    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p3, color);
    DrawLine(p3, p0, color);
}

inline void BatchRenderer::DrawAABB(const AABB3& aabb, const Vec4& color)
{
    const Point3& min = aabb.min;
    const Point3& max = aabb.max;

    Point3 p0{ min.x, min.y, min.z };
    Point3 p1{ max.x, min.y, min.z };
    Point3 p2{ max.x, max.y, min.z };
    Point3 p3{ min.x, max.y, min.z };

    Point3 p4{ min.x, min.y, max.z };
    Point3 p5{ max.x, min.y, max.z };
    Point3 p6{ max.x, max.y, max.z };
    Point3 p7{ min.x, max.y, max.z };

    // back
    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p3, color);
    DrawLine(p3, p0, color);

    // front
    DrawLine(p4, p5, color);
    DrawLine(p5, p6, color);
    DrawLine(p6, p7, color);
    DrawLine(p7, p4, color);

    // left
    DrawLine(p0, p4, color);
    DrawLine(p3, p7, color);

    // right
    DrawLine(p1, p5, color);
    DrawLine(p2, p6, color);
}
// Viewport space -> NDC -> world spcae
inline Point3 BatchRenderer::Pick(const Point2& screen_pos) const
{
    Point2 window_size = Window::Get()->GetWindowSize();

    // Screen position in NDC near plane
    Vec4 ndc_pos((2 * screen_pos.x) / window_size.x - 1, 1 - (2 * screen_pos.y) / window_size.y, -1, 1);

    // Camera space position
    Vec4 view_pos = Mul(shader.GetProjectionMatrix().GetInverse(), ndc_pos);

    // Re-stratching z and set w to 1 so that it can be interpreted as a point
    view_pos.z /= view_pos.w;
    view_pos.w = 1;

    Vec4 world_pos = Mul(shader.GetViewMatrix().GetInverse(), view_pos);

    return Point3(world_pos.x, world_pos.y, world_pos.z);
}

} // namespace alzartak