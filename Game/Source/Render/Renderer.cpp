#include <precomp.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Render/PostProcessing/Framebuffer.h>


bool Render::Renderer::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);        

    
    if ( !m_Window.Init() )
    {
        LOG_ERROR( "FAILED TO INIT MAIN WINDOW" );
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


    glfwSetFramebufferSizeCallback( m_Window.m_glfwWindow, Window::Window::FrameBufferResizeCallback );
        

    m_DebugWindow.Init();

    // stbi_set_flip_vertically_on_load(true);
     
    m_FrameBuff = new Framebuffer();
    m_FrameBuff->Init();


    if ( !m_EntryManager.Init() )
        return false;

    

    lastTime = static_cast<float>( glfwGetTime() );

    LOG_INFO("Renderer initialized");

    return true;
}

void Render::Renderer::Update()
{   
    float timeValue = static_cast<float>( glfwGetTime() );
    float delta = timeValue - lastTime;
    lastTime = timeValue;    

    m_FrameBuff->BindSceneBegin();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_EntryManager.Update(delta);

    m_FrameBuff->BindSceneEnd();

    if (m_Window.IsRunning())
    {        
        UpdateWindows();
    }        
}

GLFWwindow* Render::Renderer::GetWindow()
{
    return m_Window.m_glfwWindow;
}

bool Render::Renderer::IsRunning()
{
    return m_Window.m_running;
}

Render::Renderer::~Renderer()
{   
#ifdef _DEBUG        
    m_DebugWindow.Destroy();
#endif
    delete m_FrameBuff;    
}

void Render::Renderer::UpdateWindows()
{
#ifdef _DEBUG
    m_DebugWindow.Update();
#endif
    m_Window.Update();
}
