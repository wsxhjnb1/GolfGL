#pragma once

class Window;

class Input
{
public:

	Input() = default;
	bool Init( Window* window );
	void ProcessInput();

	inline static float lastX = WindowData::width / 2.0f;
	inline static float lastY = WindowData::height / 2.0f;
	inline static bool firstMouse = true;

private:
	Window* m_Window;
	float deltaTime;
	float lastFrame;
};