#pragma once

#include <Entities/Entity.h>
#include <Model/Model.h>

namespace Entities
{
    class Ball : public Render::Model, public Entity
    {
    public:
        Ball();

        void Update(float delta) override;

    private:
        glm::vec3 m_speed;
        glm::vec3 m_acceleration;
        glm::vec3 m_direction;
        float m_frictionFactor;
        float m_angle;
        float m_scale;
        glm::vec3 m_friction{0.f};

        unsigned m_diffuseMap;

        void m_SetLightUniforms() const;
        void m_HandleTransformations(float delta);

        [[nodiscard]] glm::vec3 m_NormalOnVec(const glm::vec3 &direction) const
        {
            return glm::normalize(glm::cross(m_speed, {0.f, 1.f, 0.f}));
        }

        inline static bool m_ShootEvent()
        {
            return glfwGetKey(Window::Window::GetGlfwWindow(), GLFW_KEY_SPACE) == GLFW_PRESS;
        }

        friend EntityManager;
#ifdef _DEBUG
        friend Window::DebugWindow;
#endif
    };
} // namespace Entities
