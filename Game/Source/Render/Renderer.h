#pragma once

#include <Entities/EntityManager.h>
#include <Window/Window.h>

#ifdef _DEBUG
#include <Window/DebugWindow.h>
#endif

namespace Render
{
    class Framebuffer;

    class Renderer
    {
    public:
        Renderer(const Renderer &) = delete;
        Renderer(Renderer &&)      = delete;
        void operator=(const Renderer &) = delete;

        bool Init();
        void Update();
        bool IsRunning();

        GLFWwindow *GetWindow();

        Renderer() = default;
        ~Renderer();
        static bool cameraFollowBall;

    private:
        float lastTime;
        Window::Window m_Window;

#ifdef _DEBUG
        Window::DebugWindow m_DebugWindow;
#endif

        Framebuffer *m_FrameBuff                = nullptr;
        Entities::EntityManager *m_EntryManager = nullptr;

        inline static float m_clearColorRgba[4] = {0.5f, 0.5f, 0.5f, 1.f};

        inline void m_clearColor();
        inline void m_UpdateWindows();
    };
} // namespace Render
