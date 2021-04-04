#include <precomp.h>

#include "Skybox.h"
#include "skyboxData.h"

Entities::Skybox::Skybox()  :	Entity("skybox")
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyData.vertices), &skyData.vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);

    texture = m_LoadCubemap();
    shader.ActivateShader();
    shader.SetValue("skybox", 0);
}

Entities::Skybox::~Skybox()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Entities::Skybox::Update(float delta)
{
    glDepthFunc(GL_LEQUAL);
    shader.ActivateShader();
    updatePV();
    
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

unsigned Entities::Skybox::m_LoadCubemap()
{    
    using rTex = Render::Texture;

    unsigned id = rTex::GenID( GL_TEXTURE_CUBE_MAP );    
    unsigned textureXPos = GL_TEXTURE_CUBE_MAP_POSITIVE_X;     

    for (auto& face : skyData.faces)
    {        
        auto rawIm{ rTex::LoadRawImage( face ) };
        if ( rawIm.data == nullptr )
        {
            std::cout << "Cubemap texture failed to load at path: " << face << std::endl;
            return 0;
        }
        
        glTexImage2D( textureXPos++, 0, GL_RGB, rawIm.width, rawIm.height, 0, GL_RGB,
                      GL_UNSIGNED_BYTE, rawIm.data );                      
         
    }

    
    rTex::SetTexParam(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    rTex::SetTexParam(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    rTex::SetTexParam(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    rTex::SetTexParam(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    rTex::SetTexParam(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return id;
}