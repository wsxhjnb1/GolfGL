#pragma once

struct WindowData
{
	inline static int width = 1280;
	inline static int height = 720;
	inline static const char* windowTitle = "Game Title";

	inline static const char* glslVersion = "#version 330";

	inline static auto GetResolution() -> std::string 
	{
		return std::to_string(width) + "x" + std::to_string(height);
	}

	inline static void ChangeResolution(int w, int h)
	{
		width = w;
		height = h;
	}
};