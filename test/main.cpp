#if defined(_WIN32)
#include <crtdbg.h>
#endif

#include "alzartak/window.h"

using namespace alzartak;

Window* window;
Vec4 clear_color{ 190.0f / 255.0f, 220.0f / 255.0f, 230.0f / 255.0f, 1.0f };

void MainLoop()
{
    window->BeginFrame(clear_color);
    ImGui::ShowDemoWindow();

    if (Input::IsKeyPressed(GLFW_KEY_SPACE))
    {
        std::cout << "wakha" << std::endl;
    }

    window->EndFrame();
}

int main()
{
#if defined(_WIN32) && defined(_DEBUG)
    // Enable memory-leak reports
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    window = Window::Init(1280, 720, "alzartak");

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(MainLoop, 0, 1);
#else
    while (!window->ShouldClose())
    {
        MainLoop();
    }
#endif

    return 0;
}