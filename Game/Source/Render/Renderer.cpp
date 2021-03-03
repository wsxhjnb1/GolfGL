#include <precomp.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Render/PostProcessing/Framebuffer.h>


auto Render::Renderer::Init() -> bool
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    

    m_Window = new Window;
    if ( !m_Window->Init() )
    {
        LOG_FATAL("Failed to init window!");
        return false;
    }


    if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) )
    {
        LOG_ERROR("Glad failed to load gl");
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);
    // glEnable(GL_STENCIL_TEST);

    glViewport( 0, 0, WindowData::width, WindowData::height ); 

    glfwSetFramebufferSizeCallback(m_Window->GetGlfwWindow(), Window::FrameBufferResizeCallback);


    // ImGui part
#ifdef _DEBUG
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();       
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();    

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_Window->GetGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init(glslVersion);
#endif // _DEBUG    

    // stbi_set_flip_vertically_on_load(true);
     
    m_FrameBuff = new Framebuffer();
    m_FrameBuff->Init();


    if (!m_EntryManager.Init())
        return false;


    lastTime = glfwGetTime();

    LOG_INFO("Renderer initialized");

    return true;
}

void Render::Renderer::Update()
{   

    float timeValue = glfwGetTime();
    float delta = timeValue - lastTime;
    lastTime = timeValue;    

    m_FrameBuff->BindSceneBegin();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_EntryManager.Update(delta);

    m_FrameBuff->BindSceneEnd();

    if (m_Window->IsRunning())
    {        
        m_Window->Update();
    }        
}

auto Render::Renderer::IsRunning() -> bool
{
    return m_Window->m_running;
}

auto Render::Renderer::GetWindow() -> Window*
{
    return m_Window;
}

Render::Renderer::~Renderer()
{           
    delete m_Window;
}
