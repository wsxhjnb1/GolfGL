#pragma once

#include <Entities/Entity.h>

namespace Entities
{
    class Skybox : public Entity
    {
    public:
        Skybox();
        ~Skybox() override;

        void Update(float delta) override;

    private:
        unsigned m_VAO, m_VBO;
        unsigned m_textureID;

        [[nodiscard]] unsigned m_LoadCubemap() const;

        // Must activate shader before usage
        void m_SetPV() const;
        
    };
} // namespace Entities
