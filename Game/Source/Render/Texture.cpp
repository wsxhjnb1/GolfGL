#include <precomp.h>

#include "Texture.h"

Render::Texture::Texture(const std::string &name, Render::TextureType type)
    : m_Name(name.substr(name.find_last_of('/') + 1)), m_Type(type)
{
    if (m_Name.empty())
    {
        m_Name = name;
        m_ID   = LoadNativeTexture(std::string{"Resources/Textures/"}.append(name));
    }

    m_ID = LoadNativeTexture(name);
}

unsigned Render::Texture::GetID() { return m_ID; }

std::string Render::Texture::GetName() { return m_Name; }

Render::TextureType Render::Texture::GetType() { return m_Type; }

unsigned Render::Texture::GenID(GLenum target)
{
    unsigned id;
    glGenTextures(1, &id);
    glBindTexture(target, id);

    return id;
}

Render::RawImage Render::Texture::LoadRawImage(const std::string &name, int desiredChannels)
{
    int width, height, nChannels; // NOLINT

    unsigned char *data = stbi_load(name.c_str(), &width, &height, &nChannels, desiredChannels);

    return {width, height, nChannels, data};
}

GLint Render::Texture::GetFormat(int nChannels)
{
    switch (nChannels)
    {
    case 2: return GL_RG;
    case 3: return GL_RGB;
    case 4: return GL_RGBA;
    default: return GL_RED;
    }
}

void Render::Texture::BindTextureImage(GLenum target, unsigned id, RawImage &rawimage, bool genmm)
{
    auto [width, height, nChannels, data] = rawimage.Get();

    auto iForm = GetFormat(nChannels);

    BindTextureImage(target, id, width, height, iForm, data, genmm);
}

void Render::Texture::BindTextureImage(GLenum target,
                                       unsigned id,
                                       int width,
                                       int height,
                                       GLint internalformat,
                                       const stbi_uc *data,
                                       bool genmm,
                                       GLenum format,
                                       GLint level,
                                       GLenum type)
{
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    if (genmm)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

template <>
inline void Render::Texture::SetTexParam(GLenum target, GLenum pname, float param)
{
    glTexParameterf(target, pname, param);
}

template <>
inline void Render::Texture::SetTexParam(GLenum target, GLenum pname, float *param)
{
    glTexParameterfv(target, pname, param);
}

unsigned Render::Texture::LoadNativeTexture(const std::string &name, GLenum target)
{
    auto id = GenID(target);
    RawImage rawIm{LoadRawImage(name)};

    if (rawIm.data == nullptr)
    {
        LOG_ERROR("Failed to load texture\nTexture: {}", name);

#ifdef _DEBUG
        LOG_ERROR("Error code: {}", glGetError());
#endif // _DEBUG

        return 0;
    }

    BindTextureImage(target, id, rawIm);

    SetTexParam(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    SetTexParam(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    SetTexParam(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    SetTexParam(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    LOG_INFO("Loaded texture {}", name);

    return id;
}

std::string Render::Texture::TypeToString(TextureType type)
{
    switch (type)
    {
    case Render::TextureType::DIFFUSE: return std::string{"texture_diffuse"};
    case Render::TextureType::NORMAL: return std::string{"texture_normal"};
    case Render::TextureType::SPECULAR: return std::string{"texture_specular"};
    case Render::TextureType::HEIGHT: return std::string{"texture_height"};
    case Render::TextureType::TEX_ERRORTYPE:
    default: break;
    }

    return std::string{"ErrorTextureType"};
}
