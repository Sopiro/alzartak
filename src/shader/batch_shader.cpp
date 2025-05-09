#include "alzartak/batch_shader.h"

namespace alzartak
{

BatchShader::BatchShader()
    : Shader(
          // Vertex shader
          R"(
        #version 100

        attribute vec3 pos;
        attribute vec4 color;

        varying vec4 out_color;

        uniform mat4 view;
        uniform mat4 proj;

        void main()
        {
           mat4 mvp = proj * view;

           out_color = color;
           gl_PointSize = 5.0;
           gl_Position = mvp * vec4(pos, 1.0);
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
    GetUniformLocation("view");
    GetUniformLocation("proj");
}

} // namespace alzartak