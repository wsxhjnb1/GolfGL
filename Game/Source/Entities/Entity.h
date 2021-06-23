#pragma once
#include <precomp.h>

#include <Render/Shader.h>
#include <Render/Texture.h>

#ifdef _DEBUG
namespace Window { class DebugWindow;}
#endif

namespace Entities
{
    class EntityManager;


    class Entity
    {
    public:
        virtual void Update(float delta) = 0;

        virtual ~Entity() { glDeleteTextures(1, &texture); }

    protected:
        Entity(const std::string &shaderName) : shader(shaderName), texture(0) { shader.Compile(); }

        Entity(const std::string &shaderName, const std::string &texturePath)
            : shader(shaderName), texture(Render::Texture::LoadNativeTexture(texturePath))
        {
            shader.Compile();
        }

        Entity(const std::string &shaderName, const unsigned textureID) : shader(shaderName), texture(textureID)
        {
            shader.Compile();
        }


        inline static glm::mat4 projection{
            glm::perspective(glm::radians(CAMERA.GetCameraZoom()), WindowData::W / WindowData::H, 0.1f, 2000.0f)};
        inline static glm::mat4 view{1.f};
        glm::mat4 model{1.f};
        glm::vec3 position{0.f};

        Render::Shader shader;
        unsigned texture;

        inline void setUniformPVM() const
        {
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
        }

        friend EntityManager;
#ifdef _DEBUG
        friend Window::DebugWindow;
#endif

    };
} // namespace Entities
