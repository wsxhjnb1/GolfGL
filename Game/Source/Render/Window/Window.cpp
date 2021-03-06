#include <precomp.h>


/* Struct for storing temporary graphics settings
 *  used in ImGui debug menu */
#ifdef _DEBUG
struct GxDataTmp
{
	int width;
	int height;

	bool vSync = false;

	bool settingsChanged = false;

	GxDataTmp()
		: width(WindowData::width), height(WindowData::height)
	{
	}	

	void SetResolution(int w, int h)
	{
		width = w;
		height = h;

		settingsChanged = true;
	}

	std::string GetResolution()
	{
		return std::to_string(width) + "x" + std::to_string(height);
	}

	void SetVsync(bool _vsync)
	{
		vSync = _vsync;
		settingsChanged = true;
	}

	void ApplyToGlobal()
	{
		WindowData::width = width;
		WindowData::height = height;


	}
};
#endif // _DEBUG


bool Window::Init()
{	
	m_glfwWindow = glfwCreateWindow(WindowData::width, WindowData::height, WindowData::windowTitle, nullptr, nullptr);
	if (m_glfwWindow == nullptr)
	{
		LOG_FATAL("Failed to initialize window");
		glfwTerminate();
		return false;
	}

	

	glfwMakeContextCurrent(m_glfwWindow);	

	m_running = true;

	return true;
}

bool Window::IsRunning()
{
	return m_running;
}

void Window::Update()
{	
	if (glfwWindowShouldClose(m_glfwWindow))
	{
		m_running = false;
		return;
	}		


	// *** ImGui part ***
#ifdef _DEBUG
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Debug window
	ImGui::Begin("Debug Window", &imguiData::debugMenu, ImGuiWindowFlags_MenuBar);

	ImGui::Text("Graphics settings:");

	static GxDataTmp gdt;		

	// Graphics settigns group
	{		
		ImGui::BeginGroup();
	
		ImGui::Text("Resolution:"); ImGui::SameLine();
		if (ImGui::BeginMenu(gdt.GetResolution().c_str()))
		{


			if (ImGui::MenuItem("1280x720", "720p")) { gdt.SetResolution(1280, 720); }
			if (ImGui::MenuItem("1920x1080", "1080p")) { gdt.SetResolution(1920, 1080); }

			ImGui::EndMenu();
		}

		ImGui::Text("Vsync:"); ImGui::SameLine();
		ImGui::Checkbox("", &gdt.vSync);

		if (ImGui::Button("Apply") && gdt.settingsChanged)
		{
			gdt.ApplyToGlobal();
		}

		ImGui::EndGroup();
	} // Graphics settigns group	


	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
	
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
#endif // _DEBUG

	glfwSwapBuffers(m_glfwWindow);	
	glfwPollEvents();
}

GLFWwindow* Window::GetGlfwWindow()
{
	return m_glfwWindow;
}

Window::~Window()
{	
#ifdef _DEBUG
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
#endif // _DEBUG

	glfwDestroyWindow(m_glfwWindow);
	glfwTerminate();
	LOG_INFO("Window closed...");
}

void Window::FrameBufferResizeCallback(GLFWwindow* glfwWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}
