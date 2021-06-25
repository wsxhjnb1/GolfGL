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
        virtual int Activate(int index) = 0;
        virtual ~Material() = default;
    protected:
        explicit Material(Render::Shader& shader);
        static void BindTexture(int& index, unsigned texID);
        Render::Shader& shader;
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
     * @brief Physically based rendering
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
        unsigned m_diffuse;
        unsigned m_normal;
        unsigned m_specular;
    public:
        BlinnPhong(Render::Shader& shader, const std::string& directory);
        ~BlinnPhong() override;
        int Activate(int index = GL_TEXTURE0) override;    
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
        unsigned m_albedo;
        unsigned m_normal;
        unsigned m_ao;
    public:
        PBR(Render::Shader& shader, const std::string& directory);
        ~PBR() override;
        int Activate(int index = GL_TEXTURE0) override;    
    };
}

