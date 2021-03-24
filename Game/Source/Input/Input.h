#pragma once

class Input
{
public:

	Input() = default;
	bool Init( GLFWwindow* window );
	void ProcessInput();

	inline static float lastX = WindowData::W / 2.0f;
	inline static float lastY = WindowData::H / 2.0f;
	inline static bool firstMouse = true;

private:
	GLFWwindow* m_Window;
	float deltaTime;
	float lastFrame;
};