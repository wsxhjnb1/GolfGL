#include <precomp.h>

void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


bool Input::Init(GLFWwindow* window)
{	
	SMASSERT( window != nullptr,  "Failed to init window");

	m_Window = window;

#ifdef _DEBUG
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#else
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif //_DEBUG

	glfwSetCursorPosCallback(m_Window, mouseCallback);
	glfwSetScrollCallback(m_Window, scrollCallback);

	return true;
}

void Input::ProcessInput()
{
	
	glfwPollEvents();

	// Esc
	if (glfwGetKey( m_Window , GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window, true);


	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	Camera::GetCamera().SetCameraSpeed(deltaTime);

	// W
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		Camera::GetCamera().LookUp();
	// S
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		Camera::GetCamera().LookDown();
	// D
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		Camera::GetCamera().LookRight();
	// A
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
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

	float x_pos = static_cast<float>( xpos );
    float y_pos = static_cast<float>( ypos );

	if (Input::firstMouse)
	{
        Input::lastX = x_pos;
        Input::lastY = y_pos;
		Input::firstMouse = false;
	}

	float xoffset = x_pos - Input::lastX;
	float yoffset = Input::lastY - y_pos; // reversed since y-coordinates go from bottom to top

	Input::lastX = x_pos;
	Input::lastY = y_pos;

	Camera::GetCamera().ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::GetCamera().ProcessMouseScroll( static_cast<float>(yoffset) );
}