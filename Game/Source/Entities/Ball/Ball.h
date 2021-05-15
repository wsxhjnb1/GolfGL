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
        glm::vec3 m_position;
        glm::vec3 m_direction;
        float m_angle;
        float m_speed;

        unsigned m_diffuseMap;

        void m_SetLightUniforms() const;
        void m_HandleTransformations(float delta);

        [[nodiscard]] glm::vec3 m_NormalOnVec(const glm::vec3 &direction) const
        {
            return glm::normalize(glm::vec3{direction.z, 0.f, -direction.x});
        }

        inline bool m_ShootEvent()
        {
            return glfwGetKey(Window::Window::GetGlfwWindow(), GLFW_KEY_SPACE) == GLFW_PRESS;
        }

#ifdef _DEBUG
        friend Window::DebugWindow;
#endif
    };
} // namespace Entities
