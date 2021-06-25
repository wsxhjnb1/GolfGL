/*
  This code was mainly copied from https://github.com/JoeyDeVries/LearnOpenGL
*/

#include <precomp.h>

#include "Mesh.h"

Render::Mesh::Mesh(std::vector<Render::Vertex> &_vertices,
                   std::vector<unsigned> &_indices,
                   std::vector<Render::Texture> &_textures)
    : vertices(_vertices), indices(_indices), textures(_textures)
{
    m_Init();
}

void Render::Mesh::Draw(Render::Shader *shader)
{
    // shader->ActivateShader();

    // bind appropriate textures
    /*
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number

        // TextureType type = textures[i].GetType();

        // now set the sampler to the correct texture unit
        // auto uni{m_TypeCounter.TextureNameFactory(textures[i].GetType())};
        // LOG_TRACE("Setting texture type: {}", uni);
        // shader->SetValue(uni, (int)i);

        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].GetID());
    }
    */
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    /*
    // always good practice to set everything back to defaults once configured.
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    */
    // glUseProgram(0);
}

void Render::Mesh::m_Init()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array
    // which again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)nullptr);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}