#pragma once

class Input
{
public:

	Input() = default;
	bool Init( GLFWwindow* window );
	void ProcessInput();

	inline static float lastX = WindowData::width / 2.0f;
	inline static float lastY = WindowData::height / 2.0f;
	inline static bool firstMouse = true;

private:
	GLFWwindow* m_Window;
	float deltaTime;
	float lastFrame;
};