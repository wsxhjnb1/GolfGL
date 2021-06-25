#include <precomp.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "DebugWindow.h"

#include <Entities/EntityManager.h>
#include <Window/Window.h>

#include <Entities/Ball/Ball.h>
#include <Entities/Ball/ballDefault.h>
#include <Entities/Terrain/Terrain.h>

/* Initialize after setting up native opengl viewport */
bool Window::DebugWindow::Init(EntMan *entityManager)
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
    m_DrawGFXSettings();
    //-------------------------------
    m_DrawCameraMenu();
    //-------------------------------
    m_DrawLightMenu();
    //-------------------------------
    if (m_EntityManager != nullptr)
    {
        m_DrawBallMenu();
    }
    //-------------------------------
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
    ImGui::Text("Light");

    ImGui::Indent(10.f);
    ImGui::BeginGroup();
    static int idx = 0;
    ImGui::SliderInt("", &idx, 0, 3);
    ImGui::InputFloat3("Light position", &LIGHT[idx].Position[0]);
    ImGui::InputFloat3("Light ambient",  &LIGHT[idx].Ambient[0]);
    ImGui::InputFloat3("Light diffuse",  &LIGHT[idx].Diffuse[0]);
    ImGui::InputFloat3("Light specular", &LIGHT[idx].Specular[0]);

    ImGui::EndGroup();
    ImGui::Indent(-10.f);
    ImGui::Separator();
}

void Window::DebugWindow::m_DrawGFXSettings() const
{
    ImGui::Text("Graphics settings");

    ImGui::Indent(10.f);
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

    if (ImGui::Checkbox("VSync", &WindowData::vsync))
    {
    }
    ImGui::Indent(-10.f);
    ImGui::Separator();
}

void Window::DebugWindow::m_DrawCameraMenu() const
{
    ImGui::Text("Camera");

    ImGui::Indent(10.f);
    ImGui::BeginGroup();

    auto& pos = CAMERA.GetCameraPos();
    ImGui::Text("Position: (%.3f, %.3f, %.3f)", pos.x, pos.y, pos.z);
    ImGui::SliderFloat("Movement speed", &CAMERA.MovementSpeed, 0.f, 50.f);
    ImGui::SliderFloat("Sensitivity", &CAMERA.MouseSensitivity, 0.f, 1.f);

    ImGui::EndGroup();
    ImGui::Indent(-10.f);
    ImGui::Separator();
}

void Window::DebugWindow::m_DrawBallMenu() const
{
    ImGui::Text("Ball");

    ImGui::Indent(10.f);
    ImGui::BeginGroup();
    auto* ball = static_cast<Entities::Ball*>(m_EntityManager->GetEntity("ball"));
    static bool attach = true;
    
    ImGui::Text("Position: %s", glm::to_string(ball->position).c_str()+4);
    if( ImGui::InputFloat3("Set position", &Entities::ballDefault::position[0]) )
    {
        ball->position = Entities::ballDefault::position;
        if(attach)
        {
            m_CorrectPosition(ball);
        }
        ball->m_UpdateModelMatrix();
        ball->m_speed  = glm::vec3{0.f};
    }
    ImGui::SameLine();
    if( ImGui::Checkbox("Attach to terrain", &attach) )
    {
        m_CorrectPosition(ball);
        ball->m_UpdateModelMatrix();
    }

    if( ImGui::InputFloat("Scale", &Entities::ballDefault::scale) )
    {
        float s = Entities::ballDefault::scale;
        ball->m_scaleMatrix = glm::scale(Math::I4, glm::vec3{s,s,s});
        ball->m_UpdateModelMatrix();
    }
    
    ImGui::BeginGroup();
        ImGui::Text("Material:");        
        ImGui::SliderFloat("shininess", &Entities::ballDefault::material_shininess, 0.f, 256.f);
    ImGui::EndGroup();

    ImGui::EndGroup();
    ImGui::Indent(-10.f);
    ImGui::Separator();
}

void Window::DebugWindow::m_CorrectPosition(Entities::Entity* entity) const
{
    auto* terrain = static_cast<Entities::Terrain*>(m_EntityManager->GetEntity("terrain"));
    terrain->CorrectPosition(entity->position.x, entity->position.z);
    entity->position.y = 3.5f + terrain->GetHeight(entity->position.x, entity->position.z);
}
#endif
