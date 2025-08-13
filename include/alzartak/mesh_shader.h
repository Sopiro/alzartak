#pragma once

#include "shader.h"

namespace alzartak
{

class MeshShader : public Shader
{
public:
    MeshShader();
    ~MeshShader() = default;

    void SetModelMatrix(const Mat4& m);
    void SetViewMatrix(const Mat4& v);
    void SetProjectionMatrix(const Mat4& p);
};

inline void MeshShader::SetModelMatrix(const Mat4& m)
{
    Use();
    SetMat4("model", m);
}

inline void MeshShader::SetViewMatrix(const Mat4& v)
{
    Use();
    SetMat4("view", v);
}

inline void MeshShader::SetProjectionMatrix(const Mat4& p)
{
    Use();
    SetMat4("proj", p);
}

} // namespace alzartak