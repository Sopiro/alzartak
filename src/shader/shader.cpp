#include "alzartak/shader.h"

namespace alzartak
{

Shader::Shader(const char* vs_code, const char* fs_code)
{
    // vertex shader
    int32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_code, NULL);
    glCompileShader(vertex_shader);

    int32 success = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(vertex_shader, 1024, nullptr, infoLog);
        std::printf("failed to compile vertex shader: %s\n", infoLog);
        exit(1);
    }

    // fragment shader
    int32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs_code, NULL);
    glCompileShader(fragment_shader);

    success = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(fragment_shader, 1024, nullptr, infoLog);
        std::printf("failed to compile fragment shader: %s\n", infoLog);
        exit(1);
    }

    // pipeline (or program)
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    success = 0;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(shader_program, 1024, nullptr, infoLog);
        std::printf("failed to link program: %s\n", infoLog);
        exit(1);
    }

    glDetachShader(shader_program, vertex_shader);
    glDetachShader(shader_program, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    shader_handle = shader_program;
}

Shader::~Shader() noexcept
{
    if (shader_handle)
    {
        glDeleteProgram(shader_handle);
    }
}

} // namespace alzartak