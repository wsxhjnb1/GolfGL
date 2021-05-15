#include "Terrain.h"
#include <precomp.h>

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

        m_UpdateModelMatrix();
        shader.ActivateShader();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        shader.SetValue("texture1", 0);
        shader.SetValue("texture2", 1);
        glUseProgram(0);
    }

    Terrain::~Terrain()
    {
        glDeleteBuffers(1, &m_EBO);
        glDeleteBuffers(1, &m_normalVBO);
        glDeleteBuffers(1, &m_textCoordsVBO);
        glDeleteBuffers(1, &m_positionVBO);
        glDeleteVertexArrays(1, &m_VAO);
    }


    void Terrain::Update(float delta)
    {
        shader.ActivateShader();

        shader.setMat4("view", CAMERA.LookAt());

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


    void Terrain::m_LoadHeightMap(const char *path)
    {
        auto &[width, height, channels, data] = Render::Texture::LoadRawImage(path, STBI_grey);

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
        m_heightMap = Math::GaussianSmoothing(m_heightMap);
        m_size      = std::min(m_heightMap.size(), m_heightMap[0].size()); // Keeping map squared

        std::vector<float> position;
        std::vector<float> normal;
        std::vector<float> textCoords;
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

                textCoords.push_back(x / 50.0f);
                textCoords.push_back(z / 50.0f);
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

        m_UpdateModelMatrix();

        m_Init(position, normal, textCoords, indices);
    }


    void Terrain::m_Init(const std::vector<float> &position,
                         const std::vector<float> &normal,
                         const std::vector<float> &textCoords,
                         const std::vector<unsigned> &indices)
    {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &m_positionVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
        glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        glGenBuffers(1, &m_textCoordsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_textCoordsVBO);
        glBufferData(GL_ARRAY_BUFFER, textCoords.size() * sizeof(float), textCoords.data(), GL_STATIC_DRAW);
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

    void Terrain::m_UpdateModelMatrix()
    {
        model = std::move(glm::mat4(m_scale, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, m_scale, 0.0f,
                                    -1.0f * m_halfSize, 0.0f, -1.0 * m_halfSize, 1.0f));
    }

} // namespace Entities