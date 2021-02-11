#include <precomp.h>

#include "Ball.h"

Ball::Ball()
	:	Render::Model("Resources/Objects/golfBall/golfBall.obj")
    , m_Shader("ball"), direction(Camera::GetCamera().GetCameraFront()), position(glm::vec3(0.f, 0.f, 0.f))
    , projection(glm::mat4(1.f)), view(glm::mat4(1.f)), model(glm::mat4{ 1.f })
    , m_speed(0.f)
{           
    m_Texture = Render::Texture::LoadNativeTexture("Resources/Objects/golfBall/diffuse.jpg");
}

Ball::~Ball()
{	
}

void Ball::Update(float delta)
{            
    projection = glm::perspective(glm::radians(Camera::GetCamera().GetCameraZoom())
        , (float)WindowData::width / (float)WindowData::height, 0.1f, 100.0f);

    view = Camera::GetCamera().LookAt();            

    if (m_moving)
    {
        m_speed -= 0.0001f;
        position = -m_speed * glm::normalize(glm::vec3{ direction.z, 0.f, -direction.x });
        model = glm::translate(glm::mat4{1.f}, position);

        if (m_timeout - delta <= 0.f)
        {
            LOG_INFO("STOP");
            m_moving = false;
        }
    }
    else if(glfwGetKey(Window::GetGlfwWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        LOG_INFO("SHOOT");
        direction = {Camera::GetCamera().GetCameraFront() };
        m_moving = true;
        m_timeout = 0.002f;
        m_speed = 0.05f;
    }                

    m_Shader.ActivateShader();
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    m_Shader.setMat4("projection", projection);
    m_Shader.setMat4("view", view);
    m_Shader.setMat4("model", model);    

    Draw(&m_Shader);
}
