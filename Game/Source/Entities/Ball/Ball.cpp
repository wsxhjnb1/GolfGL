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
        , m_scale(ballDefault::scale)
    {
        this->position = ballDefault::position;
        model          = glm::translate(glm::mat4{1.f}, position);
        model          = glm::scale(model, m_scale * glm::vec3{1.f, 1.f, 1.f});

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
            if (glm::length(rot) < 0.2f)
                rot = glm::vec3{0.f};
            else
                m_angle += 5.f * delta * glm::length(m_speed);
            // m_angle = fmod(m_angle, 360.f);

            model = glm::translate(glm::mat4{1.f}, position);
            model = glm::rotate(model, -m_angle, rot);

            model = glm::scale(model, ballDefault::scale * glm::vec3{1.f, 1.f, 1.f});
            // m_friction = std::move( m_frictionFactor * m_speed );
            // m_friction = m_frictionFactor * m_speed;
        }
        else if (m_ShootEvent())
        {
            m_speed = 2.f * glm::normalize(CAMERA.GetCameraFront());
        }
    }
} // namespace Entities
