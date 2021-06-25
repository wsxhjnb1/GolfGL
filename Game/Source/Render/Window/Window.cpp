#include <precomp.h>

namespace Window
{
    bool Window::Init()
    {
        m_glfwWindow =
            glfwCreateWindow(WindowData::width, WindowData::height, WindowData::windowTitle, nullptr, nullptr);
        SMASSERT(m_glfwWindow != nullptr, "Failed to init glfwWindow");

        glfwMakeContextCurrent(m_glfwWindow);
        glfwSwapInterval(WindowData::vsync ? 1 : 0);

        m_running = true;

        return true;
    }

    bool Window::IsRunning() const { return m_running; }

    void Window::Update()
    {
        glfwSwapInterval(WindowData::vsync ? 1 : 0);

        if (glfwWindowShouldClose(m_glfwWindow) == 0)
        {
            glfwSwapBuffers(m_glfwWindow);
            return;
        }

        m_running = false;
    }

    GLFWwindow *Window::GetGlfwWindow() { return m_glfwWindow; }

    void Window::Destroy()
    {
        glfwDestroyWindow(m_glfwWindow);
        m_running = false;
        LOG_INFO("Window closed...");
    }

    void Window::FrameBufferResizeCallback(GLFWwindow *glfwWindow, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
} // namespace Window
