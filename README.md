# alzartak

`alzartak` is a lightweight prototyping library designed for graphics applications.

## Installation

You can easily include library using CMake's FetchContent module.

```cmake
include(FetchContent)

FetchContent_Declare(
    alzartak
    GIT_REPOSITORY https://github.com/Sopiro/alzartak.git
    GIT_TAG master
)

FetchContent_MakeAvailable(alzartak)

...

target_link_libraries(${PROJECT_NAME} PRIVATE alzartak)

```

## Building and Running Sample
- Install [CMake](https://cmake.org/install/)
- Ensure CMake is in the system `PATH`
- For WebAssembly target, also install [Emscripten](https://emscripten.org/)
- Clone the repository: 
  - `git clone https://github.com/Sopiro/alzartak.git`
- Run following command based on your target platform:
  - Native build: just `build` or `build clang`
  - WebAssembly build: `build web`
- Run sample application by `run`

## Minimal Example

```c++
#include "alzartak/alzartak.h"

int main()
{
    using namespace alzartak;

    Window* window = Window::Init(1280, 720, "alzartak");
    Renderer renderer;
    Camera2D camera;

    const float scale = 100.0f;
    Vec2 extents = window->GetWindowSize() / scale;
    Mat4 projection_matrix = Mat4::Orth(-extents.x / 2.0f, extents.x / 2.0f, -extents.y / 2.0f, extents.y / 2.0f, 0.0f, 1.0f);
    renderer.SetProjectionMatrix(projection_matrix);

    while (!window->ShouldClose())
    {
        window->BeginFrame({ 0.7, 0.8, 0.9, 1 }); // Clear window

        renderer.SetViewMatrix(camera.GetCameraMatrix());

        if (Input::IsKeyPressed(GLFW_KEY_SPACE))
            std::cout << "Hello alzartak" << std::endl;

        renderer.DrawLine({ 0, 0 }, { 1, 1 });
        renderer.FlushAll();
        
        window->EndFrame();
    }

    return 0;
}
```

## Dependencies
- [GLAD](https://github.com/Dav1dde/glad): Multi-Language Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.
- [GLFW](https://github.com/glfw/glfw): A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input.
- [ImGui](https://github.com/ocornut/imgui): Bloat-free Graphical User interface for C++ with minimal dependencies.