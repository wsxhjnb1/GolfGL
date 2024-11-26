#include <precomp.h>

#include <Render/PostProcessing/Framebuffer.h>


bool Render::Renderer::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);        

    SMASSERT( m_Window.Init(), "FAILED TO INIT MAIN WINDOW" );    


    SMASSERT( gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Glad failed to load gl" );
    

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);

    glViewport( 0, 0, WindowData::width, WindowData::height );         


    glfwSetFramebufferSizeCallback( m_Window.m_glfwWindow, Window::Window::FrameBufferResizeCallback );    
     
    m_FrameBuff = new Framebuffer();
    SMASSERT( m_FrameBuff->Init(), "Failed to init framebuffer" );

    m_EntryManager = new Entities::EntityManager();
    SMASSERT( m_EntryManager->Init(), "Failed to init Entity Manager" );

#ifdef _DEBUG
    m_DebugWindow.Init(m_EntryManager);
#endif
    

    lastTime = static_cast<float>( glfwGetTime() );

    LOG_INFO("Renderer initialized");

    return true;
}

void Render::Renderer::Update()
{
    if (!m_Window.IsRunning())
        return;

    float timeValue = static_cast<float>(glfwGetTime());
    float delta = timeValue - lastTime;
    lastTime = timeValue;

    m_FrameBuff->BindSceneBegin();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_EntryManager->Update(delta);

    // Get the position of the golf ball
    glm::vec3 golfBallPosition = m_EntryManager->GetGolfBallPosition();

    // set the camera offset
    glm::vec3 cameraOffset(0.0f, 20.0f, 5.0f);

    // calculate the desired camera position
    glm::vec3 desiredCameraPosition = golfBallPosition + cameraOffset;

    // to make the camera movement smooth, we will interpolate between the current camera position and the desired camera position
    float smoothFactor = 0.1f; // smaller values will make the camera movement smoother
    glm::vec3 smoothedCameraPosition = glm::mix(CAMERA.GetCameraPos(), desiredCameraPosition, smoothFactor);

    // update the camera position
    CAMERA.SetPosition(golfBallPosition + cameraOffset);

    // make the camera look at the golf ball
    CAMERA.SetLookAt(golfBallPosition);

    m_FrameBuff->BindSceneEnd();

    m_UpdateWindows();
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
    delete m_FrameBuff;
    delete m_EntryManager;
#ifdef _DEBUG        
    m_DebugWindow.Destroy();    
#endif          
    m_Window.Destroy();     
    glfwTerminate();
}

void Render::Renderer::m_UpdateWindows()
{
#ifdef _DEBUG
    m_DebugWindow.Update();
#endif
    m_Window.Update();
}
