#include "alzartak/mesh_shader.h"

namespace alzartak
{

MeshShader::MeshShader()
    : Shader(
          // Vertex shader
          R"(
        #version 330 core

        layout(location = 0) in vec3 iPos;
        layout(location = 1) in vec3 iNormal;
        layout(location = 2) in vec3 iTangent;
        layout(location = 3) in vec2 iTexcoord;

        out vec2 texcoord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;

        void main()
        {
           mat4 mvp = proj * view * model;

           texcoord = iTexcoord;
           gl_Position = mvp * vec4(iPos, 1.0);
        }
    )",
          // Fragment shader
          R"(
        #version 330 core

        out vec4 fragColor;
        in vec2 texcoord;

        uniform sampler2D sampler;

        void main()
        {
            fragColor = texture(sampler, texcoord);
        }
    )"
      )
{
    GetUniformLocation("model");
    GetUniformLocation("view");
    GetUniformLocation("proj");
    GetUniformLocation("sampler");
}

} // namespace alzartak
