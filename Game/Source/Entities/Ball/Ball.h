#pragma once

#include <Entities/Entity.h>
#include <Model/Model.h>
#include <Render/Shader.h>

namespace Entities
{
    class Ball : public Entity
    {
    public:
        Ball();

        void Update(float delta) override;
        void Render();

    private:
        // Light & material
        Material::PBR m_material;
        void m_SetLightUniforms() const;

        // Transformations
        Render::Model m_model;
        glm::vec3 m_speed;
        glm::vec3 m_acceleration{0.f};
        float m_scale;
        float m_frictionFactor;
        float m_angle;
        glm::vec3 m_friction{0.f};
        glm::mat4 m_rotationMatrix{Math::I4};
        glm::mat4 m_scaleMatrix;

        /**
         * \brief Model matrix recalculation
         *                    
         * Model = T * R * S         
         * \par Where:
         * T: translation matrix\n
         * R: rotation matrix\n
         * S: scale matrix\n
         *         
         */
        void m_UpdateModelMatrix();
        void m_HandleTransformations(float delta);

        [[nodiscard]] glm::vec3 m_NormalOnVec(const glm::vec3 &direction) const
        {
            return glm::normalize(glm::cross(direction, {0.f, 1.f, 0.f}));
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
