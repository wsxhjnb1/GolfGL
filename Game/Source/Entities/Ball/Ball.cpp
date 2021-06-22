#include <precomp.h>

#include "Ball.h"
#include "ballDefault.h"

namespace Entities
{
    Ball::Ball()
        : Render::Model("Resources/Objects/golfBall/golfBall.obj")
        , Entity("entity")
        , m_speed(ballDefault::speed)
        , m_acceleration(0.f)
        , m_direction(0.f)
        , m_frictionFactor(ballDefault::frictionFactor)
        , m_angle(ballDefault::angle)
        , m_scaleMatrix(glm::scale(glm::mat4{1.f}, ballDefault::scale * glm::vec3(1.f,1.f,1.f)))
        // , m_prevPos(ballDefault::position)
    {
        this->position = ballDefault::position;
        model          = glm::translate(glm::mat4{1.f}, position);
        model         *= m_scaleMatrix;

        // diffuse texture is loaded in parent class
        m_diffuseMap = (*std::find_if(textures_loaded.begin(), textures_loaded.end(), [](Render::Texture &t) {
                           return t.GetType() == TDIFFUSE;
                       })).GetID();

        shader.ActivateShader();
        shader.SetValue("material.diffuse", 0);
        setUniformPVM();
    }

    void Ball::Update(float delta)
    {
        shader.ActivateShader();
        m_HandleTransformations(delta);
        m_SetLightUniforms();
        setUniformPVM();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_diffuseMap);


        Draw(&shader);

        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }

    inline void Ball::m_SetLightUniforms() const
    {

        shader.setVec3("light.position", Light::LightPosition);
        shader.setVec3("viewPos", CAMERA.GetCameraPos());
        shader.setVec3("light.ambient", Light::Ambient);
        shader.setVec3("light.diffuse", Light::Diffuse);
        shader.setVec3("light.specular", Light::Specular);

        shader.setVec3("material.specular", ballDefault::material_specular);
        shader.SetValue("material.shininess", ballDefault::material_shininess);

        shader.setMat4("view", view);
        shader.setMat4("model", model);
    }

    inline void Ball::m_HandleTransformations(float delta)
    {
        if (m_speed != glm::vec3{0.f})
        {
            auto rot = m_NormalOnVec(m_speed);            
            m_angle = (delta / 3.14f)* glm::length(m_speed);            

            m_rotationMatrix = glm::rotate(glm::mat4{1.f}, -m_angle, rot) * m_rotationMatrix;
            model = glm::translate(glm::mat4{1.f}, position) * m_rotationMatrix * m_scaleMatrix;                        
        }
        else if (m_ShootEvent())
        {
            m_speed = ballDefault::shootSpeed * glm::normalize(CAMERA.GetCameraFront());
        }
    }
} // namespace Entities
