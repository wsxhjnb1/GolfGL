#include <precomp.h>
#include "Terrain.h"

#include "Math.hpp"

float _defaultHeightFunction(float h) { return h; }
namespace Entities
{

    Terrain::Terrain(float scale) /* This texture will be on higher points */
        : Entity("terrain", "Resources/Textures/terrain/grass.png"), m_scale(scale)
    {
        m_LoadHeightMap("Resources/Textures/terrain/height_map.jpg");

        m_textures.push_back(texture);
        m_textures.push_back(Render::Texture::LoadNativeTexture("Resources/Textures/terrain/mud.png"));

        m_SetModelMatrix();
        shader.ActivateShader();
        setUniformPVM();
        shader.SetValue("texture1", 0);
        shader.SetValue("texture2", 1);
        glUseProgram(0);
    }

    Terrain::~Terrain()
    {
        glDeleteBuffers(1, &m_EBO);
        glDeleteBuffers(1, &m_normalVBO);
        glDeleteBuffers(1, &m_texCoordsVBO);
        glDeleteBuffers(1, &m_positionVBO);
        glDeleteVertexArrays(1, &m_VAO);
    }


    void Terrain::Update(float delta)
    {
        shader.ActivateShader();

        shader.setMat4("view", view);
        shader.setVec3("light.position", LIGHT.LightPosition);
        shader.setVec3("light.ambient", LIGHT.Ambient);
        shader.setVec3("light.diffuse", LIGHT.Diffuse);
        shader.setVec3("light.specular", LIGHT.Specular);
        shader.setVec3("light.direction", glm::vec3{0.f});        

        glBindVertexArray(m_VAO);
        for (int i = 0; i < m_textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        }

        glDrawElements(GL_TRIANGLES, m_indicesNum, GL_UNSIGNED_INT, nullptr);

        for (int i = 0; i < m_textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glUseProgram(0);
    }
    
    glm::vec3 Terrain::GetGradient(float x, float z) const
    {
        float h = GetHeight(x, z);

        constexpr static glm::vec2 positions[9] = {glm::vec2{0, 0},  glm::vec2{0, -1}, glm::vec2{1, -1},
                                                   glm::vec2{1, 0},  glm::vec2{1, 1},  glm::vec2{0, 1},
                                                   glm::vec2{-1, 1}, glm::vec2{-1, 0}, glm::vec2{-1, -1}};

        auto it = std::min_element(positions, positions + 9, 
            [&](const glm::vec2 &e1, const glm::vec2 &e2) {
            return GetHeight(x + e1[0], z + e1[1]) < GetHeight(x + e2[0], z + e2[1]);
        });

        float deltaH = h - GetHeight(x + it->x, z + it->y);

        if (deltaH < ms_gValue)
            return glm::vec3{0.f, 0.f, 0.f};

        return glm::normalize(glm::vec3{it->x, -deltaH, it->y});
    }


    float Terrain::m_GetHeightRaw(const int x, const int z) const
    {
        if (x < 0 || x >= m_size || z < 0 || z >= m_size)
            return 0;
        return m_heightMap[z][x];
    }

    /* Calculating height as mean value of four adjacent points */
    float Terrain::GetHeight(float x, float z) const
    {
        // translate back to model world
        x += m_halfSize;
        z += m_halfSize;
        x /= m_scale;
        z /= m_scale;

        int x1 = static_cast<int>(x);
        int x2 = static_cast<int>(x + 0.5f);
        int z1 = static_cast<int>(z);
        int z2 = static_cast<int>(z + 0.5f);

        float y1 = m_GetHeightRaw(x1, z1);
        float y2 = m_GetHeightRaw(x2, z1);
        float y3 = m_GetHeightRaw(x1, z2);
        float y4 = m_GetHeightRaw(x2, z2);

        return (y1 + y2 + y3 + y4) / 4.0f;
    }


    void Terrain::m_LoadHeightMap(const char *path)
    {
        auto [width, height, channels, data] = Render::Texture::LoadRawImage(path, STBI_grey);

        if (width != height)
        {
            LOG_ERROR("Bad dimensions for height map!");
            return;
        }

        m_size     = width;
        m_halfSize = m_scale * m_size / 2; // Recalculate middle

        m_heightMap = Math::Matrix<float>(m_size, Math::Array<float>(m_size, 0));


        for (int i = 0; i < m_size; ++i)
        {
            for (int j = 0; j < m_size; ++j)
            {
                m_heightMap[i][j] = m_HeightFunction(data[i * m_size + j]);
            }
        }

        /* Smoothing terrain surface using Gaussian blur */
        m_heightMap = std::move(Math::GaussianSmoothing(m_heightMap));
        m_size      = std::min(m_heightMap.size(), m_heightMap[0].size()); // Keeping map squared

        std::vector<float> position;
        std::vector<float> normal;
        std::vector<float> texCoords;
        std::vector<unsigned> indices;

        for (int z = 0; z < m_size; ++z)
        {
            for (int x = 0; x < m_size; ++x)
            {
                position.push_back(x);
                position.push_back(m_heightMap[z][x]);
                position.push_back(z);

                glm::vec3 temp = std::move(m_CalculateNormal(x, z));
                normal.push_back(temp.x);
                normal.push_back(temp.y);
                normal.push_back(temp.z);

                texCoords.push_back(x / 50.0f);
                texCoords.push_back(z / 50.0f);
            }
        }


        for (int z = 0; z < m_size - 5; ++z)
        {
            for (int x = 0; x < m_size - 5; ++x)
            {
                int topLeft     = z * m_size + x;
                int topRight    = topLeft + 1;
                int bottomLeft  = topLeft + m_size;
                int bottomRight = bottomLeft + 1;
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }
        m_indicesNum = indices.size();

        m_SetModelMatrix();

        m_Init(position, normal, texCoords, indices);
    }


    void Terrain::m_Init(const std::vector<float> &position,
                         const std::vector<float> &normal,
                         const std::vector<float> &texCoords,
                         const std::vector<unsigned> &indices)
    {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &m_positionVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
        glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        glGenBuffers(1, &m_texCoordsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_texCoordsVBO);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

        glGenBuffers(1, &m_normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
        glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float), normal.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }


    glm::vec3 Terrain::m_CalculateNormal(int x, int z)
    {
        float heightL = x - 1 >= 0 ? m_heightMap[z][x - 1] : 0;
        float heightR = x + 1 < m_size ? m_heightMap[z][x + 1] : 0;
        float heightD = z - 1 >= 0 ? m_heightMap[z - 1][x] : 0;
        float heightU = z + 1 < m_size ? m_heightMap[z + 1][x] : 0;
        glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);

        return glm::normalize(normal);
    }

    void Terrain::m_SetModelMatrix()
    {
        model = glm::mat4(m_scale,            0.0f,   0.0f,              0.0f, 
                          0.0f,               1.0f,   0.0f,              0.0f, 
                          0.0f,               0.0f,   m_scale,           0.0f,
                          -1.0f * m_halfSize, 0.0f,   -1.0 * m_halfSize, 1.0f);        
    }

    void Terrain::CorrectPosition(float &x, float &z) const
    {
        static const float fHalfSize = static_cast<float>(m_halfSize);

        x = std::max(x, -fHalfSize + 10);
        x = std::min(x, fHalfSize - 10);
        z = std::max(z, -fHalfSize + 10);
        z = std::min(z, fHalfSize - 10);
    }

} // namespace Entities