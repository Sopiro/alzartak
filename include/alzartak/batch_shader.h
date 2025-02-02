#pragma once

#include "common.h"
#include "shader.h"

namespace alzartak
{

class BatchShader : public Shader
{
public:
    BatchShader();
    ~BatchShader() = default;

    const Mat4& GetViewMatrix() const;
    void SetViewMatrix(const Mat4& view_matrix);

    const Mat4& GetProjectionMatrix() const;
    void SetProjectionMatrix(const Mat4& proj_matrix);

private:
    Mat4 view_matrix{ identity };
    Mat4 proj_matrix{ identity };
};

inline const Mat4& BatchShader::GetViewMatrix() const
{
    return view_matrix;
}

inline void BatchShader::SetViewMatrix(const Mat4& vm)
{
    SetMat4("view", view_matrix = vm);
}

inline const Mat4& BatchShader::GetProjectionMatrix() const
{
    return proj_matrix;
}

inline void BatchShader::SetProjectionMatrix(const Mat4& pm)
{
    SetMat4("proj", proj_matrix = pm);
}

} // namespace alzartak