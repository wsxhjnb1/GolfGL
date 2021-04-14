#include <precomp.h>

#include "Ball.h"
#include "ballDefault.h"

namespace Entities
{
    Ball::Ball()
        : Render::Model("Resources/Objects/golfBall/golfBall.obj")
        , Entity("entity")
        , position(ballDefault::position)
        , direction(CAMERA.GetCameraFront())
        , m_angle(ballDefault::angle)
        , m_speed(0.f)
    {
        // diffuse texture is loaded in parent class
        m_diffuseMap = (*std::find_if(textures_loaded.begin(), textures_loaded.end(), [](Render::Texture &t) {
                           return t.GetType() == TDIFFUSE;
                       })).GetID();

        shader.ActivateShader();
        shader.SetValue("material.diffuse", 0);
    }

    void Ball::Update(float delta)
    {
        shader.ActivateShader();
        m_SetLightUniforms();

        m_HandleTransformations(delta);        

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_diffuseMap);
        setUniformPVM();

        Draw(&shader);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    inline void Ball::m_SetLightUniforms()
    {        

        shader.setVec3("light.position", Light::LightPosition);
        shader.setVec3("viewPos", CAMERA.GetCameraPos());
        shader.setVec3("light.ambient", Light::Ambient);
        shader.setVec3("light.diffuse", Light::Diffuse);
        shader.setVec3("light.specular", Light::Specular);

        shader.setVec3("material.specular", ballDefault::material_specular);
        shader.SetValue("material.shininess", ballDefault::material_shininess);
    }

    inline void Ball::m_HandleTransformations(float delta)
    {        

        if (m_speed > 0.f)
        {
            m_speed -= delta; // Deceleration
            position += ballDefault::scalarFixer * m_speed * direction;

            /* Angle is in degrees kept between [0, 360) */
            m_angle += m_speed;
            m_angle = m_angle < 360 ? m_angle : m_angle - 360;
        }
        else if (m_ShootEvent())
        {
            direction   = CAMERA.GetCameraFront();
            direction.y = 0.f;
            direction   = glm::normalize(direction);
            m_speed     = ballDefault::speed;
        }

        model = glm::translate(glm::mat4{1.f}, position);
        model = glm::rotate(model, glm::radians(m_angle), m_NormalOnVec(direction));
    }
} // namespace Entities
