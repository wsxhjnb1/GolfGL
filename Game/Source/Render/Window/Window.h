#pragma once

namespace Render{ class Renderer; }

namespace Window {
	class Window
	{
	public:

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		void operator=(const Window&) = delete;

		static GLFWwindow* GetGlfwWindow();


		bool Init();
		bool IsRunning();
		void Update();

		Window() = default;
		~Window();

		static void FrameBufferResizeCallback(GLFWwindow* glfwWindow, int width, int height);

	private:
		inline static GLFWwindow* m_glfwWindow = nullptr;
		bool m_running = false;

		friend class Render::Renderer;

	};
}