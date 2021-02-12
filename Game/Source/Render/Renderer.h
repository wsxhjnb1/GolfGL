#pragma once

class Window;

#include <Entities/EntityManager.h>

namespace Render {				

	class Renderer
	{
	public:
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		void operator=(const Renderer&) = delete;

		bool Init();
		void Update();
		bool IsRunning();

		Window* GetWindow();

		Renderer() = default;
		~Renderer();

	private:
		float lastTime;
		Window* m_Window;		

		Entities::EntityManager m_EntryManager;
		
	};
}