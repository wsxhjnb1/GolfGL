#pragma once

#include <precomp.h>

#include <Render/Shader.h>
#include <Render/Texture.h>

namespace Render {    

    struct Vertex {
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
        std::vector<Vertex>    vertices;
        std::vector<unsigned>  indices;
        std::vector<Texture>   textures;
        unsigned VAO;

        // constructor
        Mesh(const std::vector<Render::Vertex>& _vertices, const std::vector<unsigned>& _indices, const std::vector<Render::Texture>& _textures);
        Mesh(const std::vector<Render::Vertex>& _vertices, const std::vector<unsigned>& _indices, const Render::Texture&  _textures);
        

        // render the mesh
        void Draw(Shader* shader);
        

    private:
        // render data 
        unsigned VBO, EBO;

        void m_Init();
	};

}