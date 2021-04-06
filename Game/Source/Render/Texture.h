#pragma once

#include <string>

namespace Render
{
    enum class TextureType
    {
        DIFFUSE = 0,
        NORMAL,
        SPECULAR,
        HEIGHT,
        TEX_ERRORTYPE
    };

    struct RawImage
    {
        int width, height, nChannels;
        stbi_uc *data = nullptr;

        using Properties = std::tuple<int, int, int, stbi_uc *>;

        inline Properties Get()
        {
            return {width, height, nChannels, data};
        }

        inline stbi_uc *Set(const char *path)
        {
            stbi_image_free(data);
            data = stbi_load(path, &width, &height, &nChannels, 0);
            return data;
        }

        ~RawImage()
        {
            stbi_image_free(data);
        }
    };

    class Texture
    {
    public:
        Texture(const std::string &name, TextureType type = TextureType::DIFFUSE);

        unsigned GetID();
        std::string GetName();
        Render::TextureType GetType();

        /* Static texture loading functions */
        // -------------------------------------
        /* Load texture and returns texture ID */
        static unsigned LoadNativeTexture(const std::string &name, GLenum target = GL_TEXTURE_2D);

        /* Generates ID for texture target */
        static unsigned GenID(GLenum target = GL_TEXTURE_2D);

        /* Stores image properties in RAII struct */
        static RawImage LoadRawImage(const std::string &name, int desiredChannels = 0);

        /* Converts number of channels to native enum format */
        static GLint GetFormat(int nChannels);

        /* Binds to array and specifies texture image to native lib */
        static void BindTextureImage(GLenum target, unsigned id, RawImage &rawimage, bool genmm = true);
        static void BindTextureImage(GLenum target, unsigned id, int width, int height, GLint internalformat,
                                     const stbi_uc *data, bool genmm = true, GLenum format = GL_RGB, GLint level = 0,
                                     GLenum type = GL_UNSIGNED_BYTE);

        /* Set texture parameter */
        template <typename T = int>
        static void SetTexParam(GLenum target, GLenum pname, T param);
        /* Set texture parameters */
        template <typename T = int>
        static void SetTexParam(GLenum target, GLenum pname, const T *param);

        /* Texture type parsers */
        // -------------------------------------
        /* str_ must be in format: texture_{type} */
        inline static TextureType TypeFromString(const std::string &str_)
        {
            return (TextureType)std::toupper(str_[8]);
        }

        /* Returns formated string: texture_{type} */
        static std::string TypeToString(TextureType type);

        /* string in format: texture_{_type_}N
           where N is ordinary number of texture */
        inline static std::string TypeToStringN(TextureType type, unsigned N)
        {
            return TypeToString(type) + std::to_string(N);
        }

    private:
        unsigned m_ID;
        std::string m_Name;
        TextureType m_Type;
    };

    template <typename T>
    inline void Texture::SetTexParam(GLenum target, GLenum pname, T param)
    {
        glTexParameteri(target, pname, param);
    }

    template <typename T>
    inline void Texture::SetTexParam(GLenum target, GLenum pname, const T *param)
    {
        glTexParameteriv(target, pname, param);
    }
} // namespace Render

// Aliases
#define TDIFFUSE Render::TextureType::DIFFUSE // NOLINT
#define TNORMAL Render::TextureType::NORMAL
#define TSPECULAR Render::TextureType::SPECULAR
#define THEIGHT Render::TextureType::HEIGHT
