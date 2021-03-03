#include <precomp.h>

void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


auto Input::Init(Window* window) -> bool
{
	if (window == nullptr)
	{
		LOG_FATAL("Failed to init window");
		return false;
	}

	m_Window = window;

	glfwSetInputMode(m_Window->GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPosCallback(m_Window->GetGlfwWindow(), mouseCallback);
	glfwSetScrollCallback(m_Window->GetGlfwWindow(), scrollCallback);

	return true;
}

void Input::ProcessInput()
{
	// glfwPollEvents();
	auto* glfwWindow = m_Window->GetGlfwWindow();
	// Esc
	if (glfwGetKey( glfwWindow , GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfwWindow, true);


	auto currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	Camera::GetCamera().SetCameraSpeed(deltaTime);

	// W
	if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		Camera::GetCamera().LookUp();
	// S
	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		Camera::GetCamera().LookDown();
	// D
	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		Camera::GetCamera().LookRight();
	// A
	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		Camera::GetCamera().LookLeft();
	

}


void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{	
#ifdef _DEBUG
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Input::firstMouse = true;
		return;
	}
#endif // _DEBUG	

	if (Input::firstMouse)
	{
		Input::lastX = xpos;
		Input::lastY = ypos;
		Input::firstMouse = false;
	}

	float xoffset = xpos - Input::lastX;
	float yoffset = Input::lastY - ypos; // reversed since y-coordinates go from bottom to top

	Input::lastX = xpos;
	Input::lastY = ypos;

	Camera::GetCamera().ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::GetCamera().ProcessMouseScroll(yoffset);
}