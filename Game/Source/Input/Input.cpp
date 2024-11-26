#include <precomp.h>

bool Input::Init(GLFWwindow *window)
{
    SMASSERT(window != nullptr, "Failed to init window");

    m_Window = window;

#ifdef _DEBUG
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#else
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif //_DEBUG

    glfwSetKeyCallback(m_Window, keyCallback);
    glfwSetCursorPosCallback(m_Window, mouseCallback);
    glfwSetScrollCallback(m_Window, scrollCallback);

    return true;
}

using WWindow = Window::Window;

#define KEY_PRESSED(key) (glfwGetKey(WWindow::GetGlfwWindow(), key) == GLFW_PRESS)


void Input::ProcessInput()
{
    glfwPollEvents();

    if (KEY_PRESSED(GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(m_Window, 1);
    }

    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    CAMERA.SetCameraSpeed(deltaTime);

    if (!Render::Renderer::cameraFollowBall)
    {
        // W
        if (KEY_PRESSED(GLFW_KEY_W))
        {
            CAMERA.LookAt();
        }
        // S
        if (KEY_PRESSED(GLFW_KEY_S))
        {
            CAMERA.LookDown();
        }
        // D
        if (KEY_PRESSED(GLFW_KEY_D))
        {
            CAMERA.LookRight();
        }
        // A
        if (KEY_PRESSED(GLFW_KEY_A))
        {
            CAMERA.LookLeft();
        }
    }
}

void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

void Input::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
#ifdef _DEBUG
    if (KEY_PRESSED(GLFW_KEY_LEFT_CONTROL))
    {
        Input::firstMouse = true;
        return;
    }
#endif // _DEBUG

    float x_pos = static_cast<float>(xpos);
    float y_pos = static_cast<float>(ypos);

    if (Input::firstMouse)
    {
        Input::lastX      = x_pos;
        Input::lastY      = y_pos;
        Input::firstMouse = false;
    }

    float xoffset = x_pos - Input::lastX;
    float yoffset = Input::lastY - y_pos; // reversed since y-coordinates go from bottom to top

    Input::lastX = x_pos;
    Input::lastY = y_pos;

    CAMERA.ProcessMouseMovement(xoffset, yoffset);
}

void Input::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    CAMERA.ProcessMouseScroll(static_cast<float>(yoffset));
}
