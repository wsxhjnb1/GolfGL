#include <precomp.h>
#include <Render/Shader.h>
#include "Framebuffer.h"
#include "fbData.h"

namespace Render
{
    bool Framebuffer::Init()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Data.vertices), &Data.vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)nullptr);

        shader = new Shader("screen");
        SASSERT(shader->Compile());

        shader->ActivateShader();
        shader->SetValue("screenTexture", 0);
        shader->SetValue("velocityTexture", 1); 

        glGenFramebuffers(1, &nativeFb);
        glBindFramebuffer(GL_FRAMEBUFFER, nativeFb);

        glGenTextures(1, &textureColorBuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowData::width, WindowData::height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

        glGenTextures(1, &velocityBuffer);
        glBindTexture(GL_TEXTURE_2D, velocityBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowData::width, WindowData::height, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, velocityBuffer, 0);

        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WindowData::width, WindowData::height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        GLuint attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);

        SMASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return true;
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteRenderbuffers(1, &RBO);
        glDeleteTextures(1, &velocityBuffer); 
        delete shader;
    }

    void Framebuffer::BindSceneBegin()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, nativeFb);
        glEnable(GL_DEPTH_TEST);
    }

    void Framebuffer::BindSceneEnd()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);        

        glm::mat4 currentViewMatrix = CAMERA.LookAt();
        glm::mat4 inversePreviousView = glm::inverse(CAMERA.previousViewMatrix);
        glm::mat4 cameraMotion = currentViewMatrix * inversePreviousView;

        shader->ActivateShader();
        shader->setVec2("frameBufSize", WindowData::W, WindowData::H);
        shader->SetValue("AA", WindowData::fxaa);
        shader->setMat4("cameraMotion", cameraMotion);
        shader->SetValue("motionBlurEnabled", motionBlurEnabled);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, velocityBuffer);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, Data.n_triangles);
    }
} // namespace Render
