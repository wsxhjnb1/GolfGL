#include <precomp.h>

#include "Floor.h"
#include "Render/Texture.h"

Floor::Floor()
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

    Render::Texture t = Render::Texture("Resources\\Textures\\grass.jpg");
    m_Texture = t.GetID();
}

Floor::~Floor()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Floor::Draw(Render::Shader* shader) const
{            
    shader->ActivateShader();
    glBindVertexArray(m_VAO);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glm::mat4 projection = glm::perspective(glm::radians(Camera::GetCamera().GetCameraZoom())
        , (float)WindowData::width / (float)WindowData::height, 0.1f, 100.0f);
    shader->setMat4("projection", projection);
    glm::mat4 view = Camera::GetCamera().LookAt();
    shader->setMat4("view", view);
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -0.5f, 0.f));
    model = glm::scale(model, glm::vec3(25.f, 1.f, 25.f));
    shader->setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);    
}

unsigned Floor::GetTexture()
{
    return m_Texture;
}
