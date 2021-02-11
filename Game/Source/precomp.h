#pragma once

// extern libs
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>

// Global data
#include <Data.h>

// Enums
namespace Render {
	enum class TextureType{
		  DIFFUSE = 0
		, NORMAL
		, SPECULAR
		, HEIGHT
		, TEX_ERRORTYPE
	};
}


// classes
#include <Logger/Log.h>
#include <Error.h>
#include <Window/Window.h>
#include <Input/Input.h>
#include <Render/Renderer.h>
#include <Camera/Camera.h>
#include <Render/Texture.h>


// STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <memory>
#include <exception>