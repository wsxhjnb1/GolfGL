/*
  This code was mainly copied from https://github.com/JoeyDeVries/LearnOpenGL
*/

#pragma once

#include <precomp.h>

#include <Render/Shader.h>
#include <Render/Texture.h>

namespace Render
{
    struct Vertex
    {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
    };

    class Mesh
    {
    public:
        // mesh Data
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
        std::vector<Texture> textures;
        unsigned VAO;

        // constructor
        Mesh(std::vector<Render::Vertex> &_vertices, std::vector<unsigned> &_indices,
             std::vector<Render::Texture> &_textures);

        // ~Mesh();

        // render the mesh
        void Draw(Shader *shader);

    private:
        // render data
        unsigned VBO, EBO;

        inline void m_Init();

        // Helper struct
        struct _typeCounter
        {
            /*
             * indexing:
             * diffuse  = 0;    * normal   = 2;
             * specular = 1;    * height   = 3;
             */
            unsigned counterArray[4]{0, 0, 0, 0};

            inline std::string TextureNameFactory(TextureType type)
            {
                return Texture::TypeToStringN(type, ++counterArray[(unsigned)type]);
            }
        } m_TypeCounter;
    };
} // namespace Render
