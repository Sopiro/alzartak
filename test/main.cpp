#if defined(_WIN32)
#include <crtdbg.h>
#endif

#include "alzartak/camera.h"
#include "alzartak/renderer.h"
#include "alzartak/window.h"

using namespace alzartak;

Window* window = nullptr;
Vec4 clear_color{ 190.0f / 255.0f, 220.0f / 255.0f, 230.0f / 255.0f, 1.0f };

void MainLoop()
{
    static Renderer r;
    static Camera camera;

    window->BeginFrame(clear_color);
    // ImGui::ShowDemoWindow();

    if (Input::IsKeyPressed(GLFW_KEY_SPACE))
    {
        std::cout << "wakha" << std::endl;
    }

    constexpr float scale = 100.0f;
    Vec2 extents = window->GetWindowSize() / scale;
    WakNotUsed(extents);
    Mat4 proj_matrix = Mat4::Orth(-extents.x / 2.0f, extents.x / 2.0f, -extents.y / 2.0f, extents.y / 2.0f, 0.0f, 1.0f);
    // Mat4 proj_matrix = Mat4::Perspective(DegToRad(71.0f), 16.0f / 9.0f, 0.0f, 1000.0f);

    r.SetProjectionMatrix(proj_matrix);
    r.SetViewMatrix(camera.GetCameraMatrix());

    r.SetPointSize(5);
    r.SetLineWidth(3);
    r.DrawLine({ 0, 0 }, { 1, 1 }, Vec4(1, 0, 1, 1));
    r.DrawTriangle({ 0, 0 }, { 1, 0 }, { 0.5, 1.0 }, Vec4(0, 1, 0, 1));
    r.DrawPoint({ -1, -1 }, Vec4(1, 0, 0, 1));
    r.FlushAll();

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