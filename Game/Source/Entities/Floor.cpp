#include <precomp.h>

#include "Floor.h"

namespace Entities
{
    Floor::Floor() : Entity("plain", "Resources/Textures/grass.jpg")
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glBindVertexArray(0);

        
        // model = glm::scale(model, glm::vec3(m_scale, 1.f, m_scale));
    }

    Floor::~Floor()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }

    void Floor::Update(float delta)
    {
        shader.ActivateShader();
        setUniformPVM();

        Draw();
    }

    void Floor::Draw()
    {        
        glBindVertexArray(m_VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        

        glDrawArrays(GL_TRIANGLES, 0, n_triangles);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }
} // namespace Entities
