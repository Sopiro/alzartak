#pragma once

#include "common.h"
#include "util.h"

namespace alzartak
{

class Shader : NonCopyable
{
public:
    virtual ~Shader() noexcept;

    void Use() const;

protected:
    Shader(const char* vertex_shader, const char* fragment_shader);

    std::unordered_map<std::string, GLuint> uniform_map;
    GLuint shader_handle;
};

inline void Shader::Use() const
{
    glUseProgram(shader_handle);
}

} // namespace alzartak