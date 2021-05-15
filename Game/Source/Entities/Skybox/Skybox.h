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

        [[nodiscard]] unsigned m_LoadCubemap() const;

        
    };
} // namespace Entities
