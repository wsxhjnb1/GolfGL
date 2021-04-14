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

        
    };
} // namespace Entities
