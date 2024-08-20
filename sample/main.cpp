#if defined(_WIN32)
#include <crtdbg.h>
#endif

#include "alzartak/camera.h"
#include "alzartak/renderer.h"
#include "alzartak/window.h"

using namespace alzartak;

Vec4 clear_color{ 190.0f / 255.0f, 220.0f / 255.0f, 230.0f / 255.0f, 1.0f };
const float scale = 100.0f;
float delta_time = 0.0f;

Window* window;
Renderer* renderer;
Camera2D* camera;

void UpdateProjectionMatrix()
{
    Vec2 extents = window->GetWindowSize() / scale;
    Mat4 proj_matrix = Mat4::Orth(-extents.x / 2.0f, extents.x / 2.0f, -extents.y / 2.0f, extents.y / 2.0f, 0.0f, 1.0f);
    // Mat4 proj_matrix = Mat4::Perspective(DegToRad(71.0f), 16.0f / 9.0f, 0.0f, 1000.0f);

    renderer->SetProjectionMatrix(proj_matrix);
}

void Init()
{
    window = Window::Init(1280, 720, "alzartak");
    renderer = new Renderer;
    camera = new Camera2D;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    UpdateProjectionMatrix();
    window->SetFramebufferSizeChangeCallback([&](int32 width, int32 height) -> void {
        glViewport(0, 0, width, height);
        UpdateProjectionMatrix();
    });
}

void Terminate()
{
    delete camera;
    delete renderer;
}

void Update()
{
    window->BeginFrame(clear_color);
    // ImGui::ShowDemoWindow();

    if (Input::IsKeyPressed(GLFW_KEY_SPACE))
    {
        std::cout << 1 / delta_time << std::endl;
    }

    // Camera control
    {
        if (Input::GetMouseScroll().y != 0)
        {
            camera->scale *= Input::GetMouseScroll().y < 0 ? 1.1f : 1.0f / 1.1f;
            camera->scale.x = Clamp(camera->scale.x, 0.1f, max_float);
            camera->scale.y = Clamp(camera->scale.y, 0.1f, max_float);
        }

        static bool camera_move = false;
        static Vec2 cursor_start, camera_start;

        if (!camera_move && Input::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
        {
            camera_move = true;
            cursor_start = Input::GetMousePosition();
            camera_start = camera->position;
        }
        else if (Input::IsMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
        {
            camera_move = false;
        }

        if (camera_move)
        {
            Vec2 dist = Input::GetMousePosition() - cursor_start;
            dist.x *= -camera->scale.x / scale;
            dist.y *= camera->scale.y / scale;
            camera->position = camera_start + dist;
        }
    }

    // Rendering
    renderer->SetViewMatrix(camera->GetCameraMatrix());
    renderer->SetPointSize(5);
    renderer->SetLineWidth(3);
    renderer->DrawLine({ 0, 0 }, { 1, 1 }, Vec4(1, 0, 1, 1));
    renderer->DrawTriangle({ { 0, 0 }, Vec4(1, 0, 0, 1) }, { { 1, 0 }, Vec4(0, 1, 0, 1) }, { { 0.5, 1.0 }, Vec4(0, 0, 1, 1) });
    renderer->DrawPoint({ -1, -1 }, Vec4(1, 0, 0, 1));
    renderer->FlushAll();

    window->EndFrame();
}

int main()
{
#if defined(_WIN32) && defined(_DEBUG)
    // Enable memory-leak reports
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    Init();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(Update, 0, 1);
#else
    auto last_time = std::chrono::steady_clock::now();
    const float target_frame_time = 1.0f / window->GetRefreshRate();

    while (!window->ShouldClose())
    {
        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<float> duration = current_time - last_time;
        float elapsed_time = duration.count();
        last_time = current_time;

        delta_time += elapsed_time;
        if (delta_time > target_frame_time)
        {
            Update();
            delta_time = 0;
        }
    }
#endif

    Terminate();

    return 0;
}