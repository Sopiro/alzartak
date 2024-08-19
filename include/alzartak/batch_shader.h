#pragma once

#include "common.h"
#include "shader.h"

namespace alzartak
{

class BatchShader : public Shader
{
public:
    static std::unique_ptr<BatchShader> Create();

    ~BatchShader() = default;

    const Mat4& GetViewMatrix() const;
    void SetViewMatrix(const Mat4& view_matrix);

    const Mat4& GetProjectionMatrix() const;
    void SetProjectionMatrix(const Mat4& proj_matrix);

private:
    friend class Renderer;

    BatchShader();

    Mat4 view_matrix{ identity };
    Mat4 proj_matrix{ identity };
};

inline std::unique_ptr<BatchShader> BatchShader::Create()
{
    return std::unique_ptr<BatchShader>(new BatchShader);
}

inline const Mat4& BatchShader::GetViewMatrix() const
{
    return view_matrix;
}

inline void BatchShader::SetViewMatrix(const Mat4& vm)
{
    view_matrix = vm;
    glUniformMatrix4fv(uniform_map["view"], 1, GL_FALSE, &view_matrix[0][0]);
}

inline const Mat4& BatchShader::GetProjectionMatrix() const
{
    return proj_matrix;
}

inline void BatchShader::SetProjectionMatrix(const Mat4& pm)
{
    proj_matrix = pm;
    glUniformMatrix4fv(uniform_map["proj"], 1, GL_FALSE, &proj_matrix[0][0]);
}

} // namespace alzartak