#include <precomp.h>

namespace Window {

	bool Window::Init()
	{
		m_glfwWindow = glfwCreateWindow(WindowData::width, WindowData::height, WindowData::windowTitle, nullptr, nullptr);
		if (m_glfwWindow == nullptr)
		{
			LOG_FATAL("Failed to initialize window");
			glfwTerminate();
			return false;
		}


		glfwMakeContextCurrent(m_glfwWindow);

		m_running = true;

		return true;
	}

	bool Window::IsRunning()
	{
		return m_running;
	}

	void Window::Update()
	{
		if (glfwWindowShouldClose(m_glfwWindow))
		{
			m_running = false;
			return;
		}


		glfwSwapBuffers(m_glfwWindow);
		glfwPollEvents();
	}

	GLFWwindow* Window::GetGlfwWindow()
	{
		return m_glfwWindow;
	}

	void Window::Destroy()
	{
		glfwDestroyWindow(m_glfwWindow);
		glfwTerminate();
		LOG_INFO("Window closed...");
	}

	void Window::FrameBufferResizeCallback(GLFWwindow* glfwWindow, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

} // Window