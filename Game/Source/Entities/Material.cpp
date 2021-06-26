#include <precomp.h>
#include "Material.h"

using Tex = Render::Texture;
namespace Material
{    
    Material::Material(const std::string& name) : name(name) {}

    void Material::BindTexture(int& index, const unsigned texID)
    {
        if(texID == 0)
            return;
        glActiveTexture(index++);
        glBindTexture(GL_TEXTURE_2D, texID);
    }
    

    BlinnPhong::BlinnPhong(const std::string& name) : Material(name)        
    {
    }

#define JPG(directory, map) Tex::LoadNativeTexture(directory + '/' + map + ".jpg")
#define PNG(directory, map) Tex::LoadNativeTexture(directory + '/' + map + ".png")


    bool BlinnPhong::Init(Render::Shader* _shader, const std::string& dir)
    {
        if(_shader == nullptr)
            return false;
        shader     = _shader;
        m_diffuse  = PNG(dir, "diffuse" );
        m_normal   = PNG(dir, "normal"  );
        m_specular = PNG(dir, "specular");
        return true;
    }

    bool BlinnPhong::Init(Render::Shader *_shader, const std::string &dir, float shininess)
    {
        m_shininess = shininess;
        return Init(_shader, dir);
    }

    bool BlinnPhong::Bind(int start) const
    {
        if(shader == nullptr)
            return false;
        // shader->ActivateShader();
        if(m_diffuse != 0)  shader->SetValue(name + ".diffuse",  start++);
        if(m_normal != 0)   shader->SetValue(name + ".normal",   start++);
        if(m_specular != 0) shader->SetValue(name + ".specular", start);
        shader->SetValue(name + ".shininess", m_shininess);
        // shader->DeactivateShader();
        return true;
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

    bool BlinnPhong::SetShininess(float shininess)
    {
        m_shininess = shininess;
        if(shader == nullptr)
            return false;

        shader->SetValue(name + ".shininess", shininess);
        return true;
    }


    // -----------------    PBR    ----------------- //


    PBR::PBR(const std::string& name) : Material(name) {}

    bool PBR::Init(Render::Shader* _shader, const std::string& directory)
    {
        if(_shader == nullptr)
            return false;
        shader = _shader;
        m_albedo = PNG(directory, "albedo");
        m_normal = PNG(directory, "normal");
        m_ao     = PNG(directory, "ao");        

        return true;
    }

    bool PBR::Bind(int start) const
    {
        if(shader == nullptr)
            return false;
        shader->ActivateShader();
        if(m_albedo) shader->SetValue( name + ".albedo", start++);
        if(m_normal) shader->SetValue( name + ".normal", start++);        
        if(m_ao)     shader->SetValue( name + ".ao",     start);
        shader->DeactivateShader();
        return true;
    }

    PBR::~PBR()
    {
        if(m_albedo != 0) glDeleteTextures(1, &m_albedo);
        if(m_normal != 0) glDeleteTextures(1, &m_normal);
        if(m_ao     != 0) glDeleteTextures(1, &m_ao);
    }
    

    int PBR::Activate(int index)
    {
        BindTexture(index, m_albedo);
        BindTexture(index, m_normal);
        BindTexture(index, m_ao);
        return index;
    }
}

