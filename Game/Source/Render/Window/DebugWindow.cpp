#include <precomp.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "DebugWindow.h"

#include <Window/Window.h>
#include <Entities/EntityManager.h>

#include <Entities/Ball/Ball.h>
#include <Entities/Ball/ballDefault.h>

/* Initialize after setting up native opengl viewport */
bool Window::DebugWindow::Init(EntMan* entityManager) 
{
    m_EntityManager = entityManager;

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

#ifdef _DEBUG
    m_DrawLightMenu();
    //-------------------------------
    m_DrawGFXSettings();
    //-------------------------------
    if(m_EntityManager != nullptr)
    {
        m_DrawBallMenu();
    }
#endif
    float frametime = 1000.f / ImGui::GetIO().Framerate;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", frametime, ImGui::GetIO().Framerate);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    m_ViewportsEnable = (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0;

    if (m_ViewportsEnable)
    {
        GLFWwindow *backupCurrentContext = glfwGetCurrentContext();
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

#ifdef _DEBUG
void Window::DebugWindow::m_DrawLightMenu() const
{
    ImGui::BeginGroup();

    ImGui::Text("Light:");
    ImGui::InputFloat3("Light position", &LIGHT.LightPosition[0]);
    ImGui::InputFloat3("Light ambient", &LIGHT.Ambient[0]);
    ImGui::InputFloat3("Light diffuse", &LIGHT.Diffuse[0]);
    ImGui::InputFloat3("Light specular", &LIGHT.Specular[0]);

    ImGui::EndGroup();
    ImGui::Separator();
}

void Window::DebugWindow::m_DrawGFXSettings() const
{
    ImGui::Text("Graphics settings:");
    ImGui::BeginGroup();

    if (ImGui::BeginMenu("Resolution"))
    {
        if (ImGui::MenuItem("1280x720"))
        {
            glfwSetWindowSize(Window::GetGlfwWindow(), 1280, 720);
            glViewport(0, 0, 1280, 720);
        }
        if (ImGui::MenuItem("1920x1080"))
        {
            glfwSetWindowSize(Window::GetGlfwWindow(), 1920, 1080);
            glViewport(0, 0, 1920, 1080);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Anti-Aliasing"))
    {
        if (ImGui::MenuItem("None"))
        {
            WindowData::fxaa = false;
        }
        if (ImGui::MenuItem("FXAA"))
        {
            WindowData::fxaa = true;
        }

        ImGui::EndMenu();
    }

    ImGui::EndGroup();

    if (ImGui::Checkbox("VSync", &WindowData::vsync)) {}    
    ImGui::Separator();
}

void Window::DebugWindow::m_DrawBallMenu() const
{
    ImGui::Text("Ball:");
    ImGui::BeginGroup();        
    auto* ball = static_cast<Entities::Ball *>(m_EntityManager->GetEntity("ball"));

    ImGui::InputFloat3("",						&Entities::ballDefault::position[0]);
    ImGui::SliderFloat("Speed",					&Entities::ballDefault::speed, 0.f, 40.f);
    ImGui::SliderFloat("Deacceleration",		&Entities::ballDefault::accel, 0.f, 5.f);
    ImGui::SliderFloat("Rotation Fixer",		&Entities::ballDefault::rotationFixer, 0.f, 100.f);
    ImGui::InputFloat3("Material - specular",  &Entities::ballDefault::material_specular[0]);
    ImGui::SliderFloat("Material - shininess", &Entities::ballDefault::material_shininess, 0.f, 256.f);

    if( ImGui::Button("Restart Ball") ) { ball->position = Entities::ballDefault::position; }
    ImGui::EndGroup();

    ImGui::Separator();
}
#endif
