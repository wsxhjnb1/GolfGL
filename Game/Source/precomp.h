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

// classes
#include <Logger/Log.h>
#include <Error.h>
#include <Window/Window.h>
#include <Render/Renderer.h>
#include <Camera/Camera.h>

// Global data
#include <Window/WindowData.h>
#include <Window/imguiData.h>


// STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <memory>
#include <exception>