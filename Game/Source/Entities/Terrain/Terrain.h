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
        struct TerrainMesh
        {        
            std::vector<float>    position;
            std::vector<float>    normal;
            std::vector<float>    texCoords;
            std::vector<float>    tangent;
            std::vector<float>    bitangent;
            std::vector<unsigned> indices;
        };

        inline static float m_HeightFunction(float h) { return h / 255.f * 50.f - 25.f; }

    public:
        Terrain(float scale = 1.f);

        ~Terrain() override;

        void Update(float delta) override;

        /**
         * Calculates gradient in point (x,z).
         *
         * Finds biggest descent from all possible moves
         *
         * @param x float
         * @param z float
         *
         * @return gradient 3-dim vector
         */
        [[nodiscard]] glm::vec3 GetGradient(float x, float z) const;

        [[nodiscard]] float GetHeight(float x, float z) const;
        void CorrectPosition(float &x, float &z) const;

    private:
        [[nodiscard]] float m_GetHeightRaw(int x, int z) const;
        void m_SetModelMatrix();


        GLuint m_VAO           = 0;
        GLuint m_positionVBO   = 0;
        GLuint m_normalVBO     = 0;
        GLuint m_texCoordsVBO  = 0;
        GLuint m_tangentVBO    = 0;
        GLuint m_bitangentVBO  = 0;
        
        GLuint m_EBO           = 0;

        GLuint m_indicesNum;


        int m_size;
        int m_halfSize;
        float m_scale = 1.0f;


        std::vector<unsigned> m_textures;
        std::vector<std::vector<float>> m_heightMap;

        void m_Init();
        [[nodiscard]] TerrainMesh m_LoadMesh();
        void m_LoadHeightMap(const char *path);

        /* Helper methods for loading mesh */
        // Loading Position, Normal, TexCoords into mesh
        void m_LoadPNT(TerrainMesh& mesh, int x, int z) const;
        // Calculating and loading Tangent and Bitanget into mesh
        void m_LoadTangents(TerrainMesh& mesh, int x, int z) const;
        // Loading indices
        void m_LoadIndices(TerrainMesh& mesh, int x, int z) const;

        [[nodiscard]] glm::vec3 m_CalculateNormal(int x, int z) const;
        
        /* Constants */
        // gradient tolerance
        inline const static float ms_gValue = 0.3f;
        // terrain height interval is [-absH, absH]
        inline const static float ms_absH = 25.f;
    };
} // namespace Entities