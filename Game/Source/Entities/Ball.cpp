#include <precomp.h>

#include "Ball.h"

Ball::Ball()
	:	Render::Model("Resources/Objects/golfBall/Golf Ball OBJ.obj")
    , m_Shader("ball"), direction(glm::vec3(0.f)), position(glm::vec3(0.f, -95.f, -2.f))
    , projection(glm::mat4(0.f)), view(glm::mat4(0.f))
{       
    model = glm::scale(glm::mat4(1.f), glm::vec3(0.01f));
    model = glm::translate(model, position);

    Render::Texture t{ "Resources/Objects/golfBall/diffuse.jpg" }; // check name
    m_Texture = t.GetID();
}

Ball::~Ball()
{	
}

void Ball::Update(float delta)
{        
    m_Shader.ActivateShader(); 
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    projection = glm::perspective(glm::radians(Camera::GetCamera().GetCameraZoom())
        , (float)WindowData::width / (float)WindowData::height, 0.1f, 100.0f);

    view = Camera::GetCamera().LookAt();            

    if (m_moving)
    {
        m_speed -= 0.0001f;
        position = -m_speed * glm::vec3{ direction.x, 0.f, direction.z };
        model = glm::translate(model, position);

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


    m_Shader.setMat4("projection", projection);
    m_Shader.setMat4("view", view);
    m_Shader.setMat4("model", model);    

    Draw(&m_Shader);
}
