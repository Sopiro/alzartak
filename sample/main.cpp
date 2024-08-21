#if defined(_WIN32)
#include <crtdbg.h>
#endif

#include "alzartak/camera.h"
#include "alzartak/renderer.h"
#include "alzartak/window.h"

using namespace alzartak;

Vec4 clear_color{ 190.0f / 255.0f, 220.0f / 255.0f, 230.0f / 255.0f, 1.0f };
const float scale = 100.0f;
float delta_time = 1.0f / 60.0f;

Window* window;
Renderer* renderer;
Camera2D* camera_2d;
Camera3D* camera_3d;

bool mode = true;

void UpdateProjectionMatrix()
{
    Vec2 extents = window->GetWindowSize() / scale;
    WakNotUsed(extents);
    if (mode)
    {
        Mat4 proj_matrix = Mat4::Orth(-extents.x / 2.0f, extents.x / 2.0f, -extents.y / 2.0f, extents.y / 2.0f, -1000, 1000);
        renderer->SetProjectionMatrix(proj_matrix);
    }
    else
    {
        Mat4 proj_matrix = Mat4::Perspective(DegToRad(71.0f), 16.0f / 9.0f, 0.0f, 1000.0f);
        renderer->SetProjectionMatrix(proj_matrix);
    }
}

void Init()
{
    window = Window::Init(1280, 720, "alzartak");
    renderer = new Renderer;

    camera_2d = new Camera2D;
    camera_3d = new Camera3D;

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
    delete camera_2d;
    delete camera_3d;
    delete renderer;
}

void Update()
{
    window->BeginFrame(clear_color);
    // ImGui::ShowDemoWindow();

    ImGui::SetNextWindowPos({ 4, 4 }, ImGuiCond_Once, { 0.0f, 0.0f });

    if (ImGui::Begin("alzartak", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::Checkbox("CameraMode 2D/3D", &mode))
        {
            UpdateProjectionMatrix();
        }
    }
    ImGui::End();

    // Camera control
    {
        if (mode)
        {
            camera_2d->UpdateInput(scale);
            renderer->SetViewMatrix(camera_2d->GetCameraMatrix());
        }
        else
        {
            camera_3d->UpdateInput(delta_time);
            renderer->SetViewMatrix(camera_3d->GetCameraMatrix());
        }
    }

    // Rendering
    renderer->SetPointSize(5);
    renderer->SetLineWidth(2);
    renderer->DrawLine({ 3, 0, -3 }, { 2, 1, -5 }, Vec4(1, 0, 1, 1));
    renderer->DrawTriangle(
        { { 0, 0, 0 }, Vec4(1, 0, 0, 1) }, { { 1, 0, -0.5f }, Vec4(0, 1, 0, 1) }, { { 0.5, 1.0, -1.0f }, Vec4(0, 0, 1, 1) }
    );
    renderer->DrawPoint({ -1, -1 }, Vec4(1, 0, 0, 1));

    Vec3 m = Vec3(-2, 1, -1);
    AABB a(m, m + Vec3(0.7f));

    renderer->DrawAABB(a, Vec4(1, 1, 1, 0.2f));

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