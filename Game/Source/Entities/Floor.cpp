#include <precomp.h>

#include "Floor.h"

namespace Entities {

    Floor::Floor()
        : Entity("plain", "Resources/Textures/grass.jpg")
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
        model = glm::scale(model, glm::vec3(25.f, 1.f, 25.f));
    }

    Floor::~Floor()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }

    void Floor::Update(float delta)
    {
        updatePV();

        Draw();
    }


    void Floor::Draw()
    {
        shader.ActivateShader();
        glBindVertexArray(m_VAO);
        glBindTexture(GL_TEXTURE_2D, texture);

        setUniformPVM();

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }

    
}