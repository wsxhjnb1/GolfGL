#pragma once

namespace Render{ class Renderer; }

class Window
{
public:	
	
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	void operator=(const Window&) = delete;
		
	static auto GetGlfwWindow() -> GLFWwindow*;


	auto Init() -> bool;
	auto IsRunning() -> bool;
	void Update();		

	Window() = default;
	~Window();

	static void FrameBufferResizeCallback(GLFWwindow* glfwWindow, int width, int height);

private:	
	inline static GLFWwindow* m_glfwWindow = nullptr;
	bool m_running = false;

	friend class Render::Renderer;
	
};