#include <precomp.h>
#include "Material.h"

using Tex = Render::Texture;
namespace Material
{    
    Material::Material(Render::Shader& shader) : shader(shader) {}

    void Material::BindTexture(int& index, const unsigned texID)
    {
        if(texID == 0)
            return;
        glActiveTexture(index++);
        glBindTexture(GL_TEXTURE_2D, texID);
    }
    


#define JPG(directory, map) Tex::LoadNativeTexture(directory + '/' + map + ".jpg")
#define PNG(directory, map) Tex::LoadNativeTexture(directory + '/' + map + ".png")


    BlinnPhong::BlinnPhong(Render::Shader &shader, const std::string& directory)
        : Material(shader),
        m_diffuse (PNG(directory, "diffuse" )),
        m_normal  (PNG(directory, "normal"  )),
        m_specular(PNG(directory, "specular"))
    {
        shader.ActivateShader();
        shader.SetValue("material.diffuse",  0);
        shader.SetValue("material.normal",   1);
        shader.SetValue("material.specular", 2);
        shader.DeactivateShader();
    }                        
                             
    BlinnPhong::~BlinnPhong()
    {
        glDeleteTextures(3, &m_diffuse);
    }


    int BlinnPhong::Activate(int index)
    {
        BindTexture(index, m_diffuse);
        BindTexture(index, m_normal);
        BindTexture(index, m_specular);
        return index;
    }

    PBR::PBR(Render::Shader& shader, const std::string& directory)
        : Material(shader)
        , m_albedo(PNG(directory, "albedo"))
        , m_normal(PNG(directory, "normal"))
        , m_ao    (PNG(directory, "ao"))
    {
        shader.ActivateShader();
        shader.SetValue("material.albedo",    0);
        shader.SetValue("material.normal",    1);        
        shader.SetValue("material.ao",        2);
        shader.DeactivateShader();
    }

    PBR::~PBR()
    {
        glDeleteTextures(3, &m_albedo);
    }

    int PBR::Activate(int index)
    {
        BindTexture(index, m_albedo);
        BindTexture(index, m_normal);
        BindTexture(index, m_ao);
        return index;
    }
}

