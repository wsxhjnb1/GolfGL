#include <precomp.h>
#include "Ball.h"
#include "ballDefault.h"

namespace Entities
{
    Ball::Ball()        
        : Entity("pbr")
        , m_material(shader, ballDefault::modelDir)
        , m_model(ballDefault::modelPath)
        , m_speed(ballDefault::speed)
        , m_acceleration(0.f)
        , m_direction(0.f)
        , m_scale(ballDefault::scale)
        , m_frictionFactor(ballDefault::frictionFactor)
        , m_angle(ballDefault::angle)        
        , m_scaleMatrix(glm::scale(Math::I4, ballDefault::scale * Math::e3) )
    {
        this->position = ballDefault::position;        
        m_UpdateModelMatrix();

        // diffuse texture is loaded in parent class
        for(auto& tex : m_model.GetTextures())
            LOG_TRACE("Texture {}, {}, type {}", tex.GetID(), tex.GetName(), tex.GetType());

        shader.ActivateShader();
        setUniformPVM();        
        shader.DeactivateShader();
    }

    void Ball::Update(float delta)
    {
        shader.ActivateShader();
        m_HandleTransformations(delta);
        m_SetLightUniforms();
        setUniformPVM();

        int index = m_material.Activate(GL_TEXTURE0);
        m_model.Draw(&shader);
        Material::UnbindTextures(index);                

        shader.DeactivateShader();
    }

    inline void Ball::m_SetLightUniforms() const
    {
        for(int i = 0; i < 4; i++)
        {
            shader.setVec3("light[" + std::to_string(i) + "].position", LIGHT[i].Position);
            shader.setVec3("light[" + std::to_string(i) + "].color", LIGHT[i].Color);            
        }
        shader.setVec3("viewPos", CAMERA.GetCameraPos());

        /*
        shader.setVec3("light.ambient", Light::Ambient);
        shader.setVec3("light.diffuse", Light::Diffuse);
        shader.setVec3("light.specular", Light::Specular);
        */
        // shader.setVec3("material.specular", ballDefault::material_specular);
        // shader.SetValue("material.shininess", ballDefault::material_shininess);
    }

    void Ball::m_UpdateModelMatrix()
    {
        model = glm::translate(Math::I4, position) * m_rotationMatrix * m_scaleMatrix;
    }

    inline void Ball::m_HandleTransformations(float delta)
    {
        if (m_speed != glm::vec3{0.f})
        {
            auto rot = m_NormalOnVec(m_speed);            
            m_angle = (delta / 3.14f)* glm::length(m_speed);            

            m_rotationMatrix = glm::rotate(Math::I4, -m_angle, rot) * m_rotationMatrix;
            m_UpdateModelMatrix();
        }
        else if (m_ShootEvent())
        {
            m_speed = ballDefault::shootSpeed * glm::normalize(CAMERA.GetCameraFront());
        }
    }
} // namespace Entities
