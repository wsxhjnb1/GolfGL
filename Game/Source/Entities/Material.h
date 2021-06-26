#pragma once

// Forward declaration
namespace Render{class Shader;}

/* Material objects are intended to be used as core module of Entity */
namespace Material
{
    /** @class Material
     * Abstract base class for materials
     *
     * \par Shading models
     * - Blinn-Phong
     * - PBR
     */
    class Material
    {
    public:
        // Shader must be activated before calling function
        virtual bool Init(Render::Shader* _shader, const std::string& dir) = 0;
        virtual int Activate(int index) = 0;
        virtual ~Material() = default;
    protected:
        explicit Material(const std::string& name);
        std::string name;
        static void BindTexture(int& index, unsigned texID);
        Render::Shader* shader = nullptr;
    };

    inline void UnbindTextures(int& index)
    {
        while (index >= GL_TEXTURE0)
        {
            glActiveTexture(--index);
            glBindTexture(GL_TEXTURE_2D, 0);            
        }
    }

    /** @class BlinnPhong
     *
     * @brief Blinn-Phong shading model material
     * \par Texture maps:     
     * - Diffuse
     * - Normal
     * - Specular    
     *
     * @param shader     Reference to entity's shader
     * @param directory  Directory where maps are
     *
     */
    class BlinnPhong : Material
    {
        unsigned m_diffuse = 0;
        unsigned m_normal = 0;
        unsigned m_specular = 0;
        float    m_shininess = 256.f;
    public:
        explicit BlinnPhong(const std::string& name);
        bool Init(Render::Shader* _shader, const std::string& dir) override;
        bool Init(Render::Shader* _shader, const std::string& dir, float shininess);
        bool Bind(int start = 0) const;
        ~BlinnPhong() override;
        int Activate(int index = GL_TEXTURE0) override;
        bool SetShininess(float shininess);
    };

    /** @class PBR
     *
     * @brief Physically based rendering
     * \par Texture maps:     
     * - Albedo
     * - Normal     
     * - Ambient Occlusion
     *
     * @param shader     Reference to entity's shader
     * @param directory  Directory where maps are
     *
     */
    class PBR : Material
    {
        unsigned m_albedo = 0;
        unsigned m_normal = 0;
        unsigned m_ao = 0;
    public:
        explicit PBR(const std::string& name);
        ~PBR() override;
        bool Init(Render::Shader* _shader, const std::string& directory) override;
        bool Bind(int start = 0) const;
        int Activate(int index = GL_TEXTURE0) override;    
    };
}

