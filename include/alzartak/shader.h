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

    void SetBool(const std::string& name, bool value);
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec2(const std::string& name, const Vec2& value);
    void SetVec3(const std::string& name, const Vec3& value);
    void SetVec4(const std::string& name, const Vec4& value);
    void SetMat4(const std::string& name, const Mat4& value);

protected:
    Shader(const char* vertex_shader, const char* fragment_shader);

    GLint GetUniformLocation(const std::string& name);

private:
    std::unordered_map<std::string, GLint> uniform_map;
    GLuint shader_handle;
};

inline void Shader::Use() const
{
    glUseProgram(shader_handle);
}

inline void Shader::SetBool(const std::string& name, bool value)
{
    WakAssert(uniform_map.contains(name));
    glUniform1i(uniform_map[name], (GLint)value);
}

inline void Shader::SetInt(const std::string& name, int value)
{
    WakAssert(uniform_map.contains(name));
    glUniform1i(uniform_map[name], value);
}

inline void Shader::SetFloat(const std::string& name, float value)
{
    WakAssert(uniform_map.contains(name));
    glUniform1f(uniform_map[name], value);
}

inline void Shader::SetVec2(const std::string& name, const Vec2& value)
{
    WakAssert(uniform_map.contains(name));
    glUniform2fv(uniform_map[name], 1, &value.x);
}

inline void Shader::SetVec3(const std::string& name, const Vec3& value)
{
    WakAssert(uniform_map.contains(name));
    glUniform3fv(uniform_map[name], 1, &value.x);
}

inline void Shader::SetVec4(const std::string& name, const Vec4& value)
{
    WakAssert(uniform_map.contains(name));
    glUniform4fv(uniform_map[name], 1, &value.x);
}

inline void Shader::SetMat4(const std::string& name, const Mat4& value)
{
    WakAssert(uniform_map.contains(name));
    glUniformMatrix4fv(uniform_map[name], 1, GL_FALSE, &value.ex.x);
}

} // namespace alzartak