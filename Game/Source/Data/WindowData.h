#pragma once

struct WindowData
{
	inline static int width = 1280;
	inline static int height = 720;
	inline static const char* windowTitle = "GolfGL";
	inline static const char* glslVersion = "#version 330";
	inline static bool vsync = false;

	inline static std::string GetResolution() 
	{
		return std::to_string(width) + "x" + std::to_string(height);
	}
	
};