#pragma once

class Window;

#include <Entities/EntityManager.h>

namespace Render {	

	class Framebuffer;

	class Renderer
	{
	public:
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		void operator=(const Renderer&) = delete;

		auto Init() -> bool;
		void Update();
		auto IsRunning() -> bool;

		auto GetWindow() -> Window*;

		Renderer() = default;
		~Renderer();

	private:
		float lastTime;
		Window* m_Window;
		Framebuffer* m_FrameBuff;

		Entities::EntityManager m_EntryManager;
		
	};
}