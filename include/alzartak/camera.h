#pragma once

#include "common.h"
#include "input.h"
#include "window.h"

namespace alzartak
{

struct Camera2D
{
    Point2 position{ 0 };
    float rotation{ 0 };
    Point2 scale{ 1 };

    void UpdateInput(float screen_scale)
    {
        if (Input::GetMouseScroll().y != 0)
        {
            scale *= Input::GetMouseScroll().y < 0 ? 1.1f : 1.0f / 1.1f;
            scale.x = Clamp(scale.x, 0.1f, max_float);
            scale.y = Clamp(scale.y, 0.1f, max_float);
        }

        static bool camera_move = false;
        static Vec2 cursor_start, camera_start;

        if (!camera_move && Input::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
        {
            camera_move = true;
            cursor_start = Input::GetMousePosition();
            camera_start = position;
        }
        else if (Input::IsMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
        {
            camera_move = false;
        }

        if (camera_move)
        {
            Vec2 dist = Input::GetMousePosition() - cursor_start;
            dist.x *= -scale.x / screen_scale;
            dist.y *= scale.y / screen_scale;
            position = camera_start + dist;
        }
    }

    Mat4 GetCameraMatrix() const
    {
        // Inverse scale
        Mat4 m{ Vec4(1 / scale.x, 1 / scale.y, 1, 1) };

        // Inverse rotation
        m = MulT(Mat4(Quat::FromEuler({ 0, 0, rotation }), Vec3::zero), m);

        // Inverse translation
        m = m.Translate({ -position.x, -position.y, 0 });

        return m;
    }
};

struct Camera3D
{
    Point3 position{ 0 };
    Point3 rotation{ 0 };
    Point3 scale{ 1 };

    Vec3 velocity{ 0 };
    float speed = 0.2f;
    float sensitivity = 0.1f;
    float damping = 10.0f;

    void UpdateInput(float dt)
    {
        Window* window = Window::Get();

        if (!ImGui::GetIO().WantCaptureMouse && Input::IsMousePressed(GLFW_MOUSE_BUTTON_LEFT))
            window->SetCursorHidden(true);
        else if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
            window->SetCursorHidden(false);

        Vec3 a(0);

        if (Input::IsKeyDown(GLFW_KEY_W)) a.z--;
        if (Input::IsKeyDown(GLFW_KEY_S)) a.z++;
        if (Input::IsKeyDown(GLFW_KEY_A)) a.x--;
        if (Input::IsKeyDown(GLFW_KEY_D)) a.x++;
        if (Input::IsKeyDown(GLFW_KEY_SPACE)) a.y++;
        if (Input::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || Input::IsKeyDown(GLFW_KEY_C)) a.y--;

        if (window->GetCursorHidden())
        {
            Vec2 ma = Input::GetMouseAcceleration();
            rotation.y -= ma.x * DegToRad(sensitivity);
            rotation.x -= ma.y * DegToRad(sensitivity);
        }

        // Neck constraints
        if (RadToDeg(rotation.x) > 89) rotation.x = DegToRad(89);
        if (RadToDeg(rotation.x) < -89) rotation.x = DegToRad(-89);

        float cos = std::cos(rotation.y);
        float sin = std::sin(rotation.y);

        velocity.x += speed * (a.x * cos + a.z * sin);
        velocity.z += speed * (a.x * -sin + a.z * cos);
        velocity.y += a.y * speed;

        position += velocity * dt;
        velocity *= std::exp(-damping * dt);
    }

    Mat4 GetCameraMatrix() const
    {
        // Inverse scale
        Mat4 m{ Vec4(1 / scale, 1) };

        // Inverse rotation
        m = MulT(Mat4(Quat::FromEuler(rotation), Vec3::zero), m);

        // Inverse translation
        m = m.Translate(-position);

        return m;
    }
};

} // namespace alzartak