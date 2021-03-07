#pragma once

#include <Window/Window.h>
#include <Window/DebugWindow.h>
#include <Entities/EntityManager.h>

namespace Render {	

	class Framebuffer;

	class Renderer
	{
	public:
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		void operator=(const Renderer&) = delete;

		bool Init();
		void Update();
		bool IsRunning();

		GLFWwindow* GetWindow();

		Renderer() = default;
		~Renderer();

	private:
		float lastTime;
		Window::Window m_Window;
		Window::DebugWindow m_DebugWindow;

		Framebuffer* m_FrameBuff;
		Entities::EntityManager m_EntryManager;
		

		inline void UpdateWindows();
	};
}