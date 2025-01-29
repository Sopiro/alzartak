#pragma once

#include "common.h"
#include "shader.h"

namespace alzartak
{

class MeshShader : public Shader
{
public:
    static std::unique_ptr<MeshShader> Create();

    ~MeshShader() = default;

    void SetModelMatrix(const Mat4& m);
    void SetViewMatrix(const Mat4& v);
    void SetProjectionMatrix(const Mat4& p);

private:
    MeshShader();
};

inline std::unique_ptr<MeshShader> MeshShader::Create()
{
    return std::unique_ptr<MeshShader>(new MeshShader);
}

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