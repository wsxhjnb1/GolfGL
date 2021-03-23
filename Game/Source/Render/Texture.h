#pragma once

#include <string>



namespace Render {	

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
        unsigned id = 0;
        int width, height, nChannels;
        stbi_uc* data = nullptr;

		using Properties = std::tuple<unsigned, int, int, int, stbi_uc*>;
        inline Properties Get()
        {
            return { id, width, height, nChannels, data };
        }

		
		
		~RawImage()
        {            
            stbi_image_free( data );
        }
    };

	class Texture
	{
	public:	

		Texture(const std::string& name, TextureType type = TextureType::DIFFUSE);

		unsigned GetID();
		std::string GetName();		
		Render::TextureType GetType();
		


		/* Static texture loading functions */
		// -------------------------------------
        /* Load texture and returns texture ID */
		static unsigned LoadNativeTexture(const std::string& name, GLenum target = GL_TEXTURE_2D);	
		
		/* Stores image properties in RAII struct */				
		static RawImage LoadRawImage( const std::string &name, int desiredChannels = 0 );

		/* Converts number of channels to native enum format */
		static GLint GetFormat( int nChannels );

		/* Binds to array and specifies texture image to native lib */
        static void BindTextureImage( GLenum target, RawImage& rawimage );
        static void BindTextureImage( GLenum target, int id, int width, int height, GLint internalformat,
                                      const stbi_uc *data, GLenum format = GL_RGB, GLint level = 0, 
									  GLenum type = GL_UNSIGNED_BYTE, bool genmm = true );

		/* Set texture parameter */
		template<typename T>
        static void SetTexParam( GLenum target, GLenum pname, T param );
        /* Set texture parameters */
        template <typename T> 
		static void SetTexParam( GLenum target, GLenum pname, T* param );


		/* Texture type parsers */
        // -------------------------------------
		/* str_ must be in format: texture_{type} */
		inline static TextureType TypeFromString(const std::string& str_)
		{
            return (TextureType)std::toupper( str_[8] );
		}

		/* Returns formated string: texture_{type} */
		inline static std::string TypeToString(TextureType type)
		{
			switch (type)
			{
			case Render::TextureType::DIFFUSE:
				return std::string{ "texture_diffuse" };
			case Render::TextureType::NORMAL:
				return std::string{ "texture_normal" };
			case Render::TextureType::SPECULAR:
				return std::string{ "texture_specular" };
			case Render::TextureType::HEIGHT:
				return std::string{ "texture_height" };
			case Render::TextureType::TEX_ERRORTYPE:				
			default:
				break;
			}

			return std::string{ "ErrorTextureType" };
		}

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
inline void Texture::SetTexParam( GLenum target, GLenum pname, T param )
{
    glTexParameteri( target, pname, param );
}

template <typename T> 
inline void Texture::SetTexParam( GLenum target, GLenum pname, T* param )
{
    glTexParameteriv( target, pname, param );
}


} // Redner


#define TDIFFUSE   Render::TextureType::DIFFUSE
#define TNORMAL    Render::TextureType::NORMAL
#define TSPECULAR  Render::TextureType::SPECULAR
#define THEIGHT    Render::TextureType::HEIGHT
