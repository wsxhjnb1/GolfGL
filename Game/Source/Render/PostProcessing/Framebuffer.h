#pragma once
#pragma once

namespace Render
{
    class Shader;

    class Framebuffer
    {
    public:
        ~Framebuffer();

        bool Init();

        void BindSceneBegin();
        void BindSceneEnd();

    private:
        Shader *shader;

        unsigned VAO, VBO, RBO;
        unsigned nativeFb;
        unsigned textureColorBuffer;
        unsigned velocityBuffer;
    };
} // namespace Render
