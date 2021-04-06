#pragma once

#include <Entities/Entity.h>

namespace Entities
{
    class Skybox : public Entity
    {
    public:
        Skybox();
        ~Skybox();

        void Update(float delta) override;

    private:
        unsigned m_VAO, m_VBO;

        unsigned m_LoadCubemap();

        inline void updatePV() override
        {
            projection =
                glm::perspective(glm::radians(CAMERA.GetCameraZoom()), WindowData::W / WindowData::H, 0.1f, 100.0f);

            view = glm::mat4(glm::mat3(CAMERA.LookAt()));
        }
    };
} // namespace Entities
