#pragma once

/*
 * Generating terrain from height map
 * Terrain loading from raw pixel reading
 * Smoothing with Gaussian blur
 * Multitexture height based terrain
 */
#include <Entities/Entity.h>
namespace Entities
{
    class Terrain : public Entity
    {
        inline static float m_HeightFunction(float h) { return h / 255.f * 100.f - 50.f; }

    public:
        Terrain(float scale = 2.f);

        ~Terrain() override;

        void Update(float delta) override;


    private:
        void m_UpdateModelMatrix();


        GLuint m_VAO           = 0;
        GLuint m_positionVBO   = 0;
        GLuint m_textCoordsVBO = 0;
        GLuint m_normalVBO     = 0;
        GLuint m_EBO           = 0;

        GLuint m_indicesNum;


        int m_size;
        int m_halfSize;
        float m_scale = 1.0f;


        std::vector<unsigned> m_textures;
        std::vector<std::vector<float>> m_heightMap;


        void m_LoadHeightMap(const char *path);
        [[nodiscard]] glm::vec3 m_CalculateNormal(int x, int z);

        void m_Init(const std::vector<float> &position,
                    const std::vector<float> &normal,
                    const std::vector<float> &textCoords,
                    const std::vector<unsigned> &indices);
    };
} // namespace Entities