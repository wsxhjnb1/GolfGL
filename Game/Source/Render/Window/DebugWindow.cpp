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
	
	DrawMenu();	

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


/* Not changing anything just for decoration now */
void Window::DebugWindow::DrawMenu()
{
	ImGui::Begin("Debug Window", &m_DebugMenuActive, ImGuiWindowFlags_MenuBar);
	

		ImGui::Text("Graphics settings:");		
		{
			ImGui::BeginGroup();

			ImGui::Text("Resolution:"); ImGui::SameLine();
			if (ImGui::BeginMenu(WindowData::GetResolution().c_str()))
			{

				if (ImGui::MenuItem("1280x720", "720p")) {  }
				if (ImGui::MenuItem("1920x1080", "1080p")) {  }

				ImGui::EndMenu();
			}

			ImGui::Text("Vsync:"); ImGui::SameLine();
			ImGui::Checkbox("", &WindowData::vsync);

			if (ImGui::Button("Apply"))
			{
				// Future work
			}

			ImGui::EndGroup();
		} // Graphics settigns group	

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();	
}

