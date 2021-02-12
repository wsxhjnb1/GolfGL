#include <precomp.h>

#include "Ball.h"

namespace Entities {

    Ball::Ball()
        : Render::Model("Resources/Objects/golfBall/golfBall.obj")
        , Entity("entity", "Resources/Objects/golfBall/diffuse.jpg")
        , direction(Camera::GetCamera().GetCameraFront()), position(glm::vec3(0.f, 0.f, 0.f))
        , projection(glm::mat4(1.f)), view(glm::mat4(1.f)), model(glm::mat4{ 1.f })
        , m_speed(0.f)
    {        
    }

    Ball::~Ball()
    {
    }

    void Ball::Update(float delta)
    {
        updatePV();

        if (m_moving)
        {
            m_speed -= 0.0001f;
            position = -m_speed * glm::normalize(glm::vec3{ direction.z, 0.f, -direction.x });
            model = glm::translate(glm::mat4{ 1.f }, position);

            if (m_timeout - delta <= 0.f)
            {
                LOG_INFO("STOP");
                m_moving = false;
            }
        }
        else if (glfwGetKey(Window::GetGlfwWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            LOG_INFO("SHOOT");
            direction = { Camera::GetCamera().GetCameraFront() };
            m_moving = true;
            m_timeout = 0.002f;
            m_speed = 0.05f;
        }

        shader.ActivateShader();
        glBindTexture(GL_TEXTURE_2D, texture);

        setUniformPVM();                

        Draw(&shader);
    }
}