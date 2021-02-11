#include <precomp.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Entities/Ball.h>
#include <Entities/Floor.h>


bool Render::Renderer::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    
    const char* glslVersion = "#version 330";

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
    glEnable(GL_STENCIL_TEST);
    // glDepthFunc(GL_ALWAYS);


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
    
    m_FloorShader = new Shader("floor");
    m_ScreenShader = new Shader("screen");
    m_SkyboxShader = new Shader("skybox");

    m_Model = new Ball();
    m_Floor = new Floor(); 

    cubemapTexture = loadCubemap(faces);

    m_SkyboxShader->ActivateShader();
    m_SkyboxShader->SetValue("skybox", 0);

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);



    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


    
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowData::width, WindowData::height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);


    
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WindowData::width, WindowData::height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    lastTime = glfwGetTime();

    LOG_INFO("Renderer initialized");

    return true;
}

void Render::Renderer::Update()
{   

    float timeValue = glfwGetTime();
    float delta = timeValue - lastTime;
    lastTime = timeValue;

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Entities
    m_Model->Update(delta);
    m_Floor->Draw(m_FloorShader);


    // Skybox
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    m_SkyboxShader->ActivateShader();
    auto view = glm::mat4(glm::mat3(Camera::GetCamera().LookAt())); // remove translation from the view matrix
    glm::mat4 projection = glm::perspective(glm::radians(Camera::GetCamera().GetCameraZoom()), (float)WindowData::width / (float)WindowData::height, 0.1f, 100.0f);
    m_SkyboxShader->setMat4("view", view);
    m_SkyboxShader->setMat4("projection", projection);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default


    // Framebuffer
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);

    m_ScreenShader->ActivateShader();
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);


    if (m_Window->IsRunning())
    {        
        m_Window->Update();
    }        
}

bool Render::Renderer::IsRunning()
{
    return m_Window->m_running;
}

Window* Render::Renderer::GetWindow()
{
    return m_Window;
}

Render::Renderer::~Renderer()
{       
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteVertexArrays(1, &skyboxVBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteVertexArrays(1, &quadVBO);
    delete m_Model;        
    delete m_FloorShader;
    delete m_ScreenShader;
    delete m_SkyboxShader;
    delete m_Window;
}


unsigned int Render::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}