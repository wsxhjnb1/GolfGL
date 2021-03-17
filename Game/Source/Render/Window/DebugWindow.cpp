#include <precomp.h>

#include "DebugWindow.h"

#include <Window/Window.h>


/* Initialize after setting up native opengl viewport */
bool Window::DebugWindow::Init()
{	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	
	// Setup Platform/Renderer backends
	bool retval = ImGui_ImplGlfw_InitForOpenGL(Window::GetGlfwWindow(), true);
	return retval && ImGui_ImplOpenGL3_Init(WindowData::glslVersion);   
}



bool Window::DebugWindow::Update()
{	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	

	ImGui::Begin("Debug Window", &m_DebugMenuActive, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize);

		DrawLightMenu();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backupCurrentContext);
	}

	return true;
}

void Window::DebugWindow::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	m_DebugMenuActive = false;
}


void Window::DebugWindow::DrawLightMenu()
{				
	ImGui::BeginGroup();

		ImGui::Text("Light:"); 
		ImGui::InputFloat3("Light position", &LIGHT.LightPosition[0]);
		ImGui::InputFloat3("Light ambient",  &LIGHT.Ambient[0]);
		ImGui::InputFloat3("Light diffuse",  &LIGHT.Diffuse[0]);
		ImGui::InputFloat3("Light specular", &LIGHT.Specular[0]);

	ImGui::EndGroup();
}

