#include "alzartak/batch_shader.h"

namespace alzartak
{

BatchShader::BatchShader()
    : Shader(
          // Vertex shader
          R"(
        #version 100

        attribute vec2 pos;
        attribute vec4 color;

        varying vec4 out_color;

        uniform mat4 view;
        uniform mat4 proj;

        void main()
        {
           mat4 mvp = proj * view;

           out_color = color;
           gl_PointSize = 5.0;
           gl_Position = mvp * vec4(pos, 0.0, 1.0);
        }
    )",
          // Fragment shader
          R"(
        #version 100
        precision mediump float;

        varying vec4 out_color;

        void main()
        {
            gl_FragColor = out_color;
        }
    )"
      )
{
    uniform_map.emplace("color", glGetUniformLocation(shader_handle, "color"));
    uniform_map.emplace("model", glGetUniformLocation(shader_handle, "model"));
    uniform_map.emplace("view", glGetUniformLocation(shader_handle, "view"));
    uniform_map.emplace("proj", glGetUniformLocation(shader_handle, "proj"));
}

} // namespace alzartak