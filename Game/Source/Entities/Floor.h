#pragma once

#include <Entities/Entity.h>

namespace Entities
{
    class Floor : public Entity
    {
    public:
        Floor();
        ~Floor();

        void Update(float delta);

    private:
        inline static float planeVertices[30] = {
            // positions            // texture Coords
            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
            5.0f, -0.5f, -5.0f, 2.0f, 2.0f
        };

        unsigned int m_VAO, m_VBO;
        float m_scale = 25.f;
        int n_triangles = 6;

        inline void Draw();
    };
} // namespace Entities
