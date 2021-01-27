#pragma once

class Window;
class Floor;

namespace Render {		

	class Model;
	class Shader;	

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
		Window m_Window;		


	};
}