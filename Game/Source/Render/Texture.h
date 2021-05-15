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
        int width, height, nChannels;
        stbi_uc* data = nullptr;

		using Properties = std::tuple<int, int, int, stbi_uc*>;
        Properties Get()
        {
            return { width, height, nChannels, data };
        }

		stbi_uc* Set( const char *path, int desiredChannels = 0 )
        {
            stbi_image_free( data );
            data = stbi_load( path, &width, &height, &nChannels, desiredChannels);
            return data;
        }
		
		~RawImage()
        {            
            stbi_image_free( data );
        }
    };

	class Texture
	{
	public:	

		Texture(const std::string& name, TextureType type = TextureType::DIFFUSE, GLenum target = GL_TEXTURE_2D);

		unsigned GetID() const;
		std::string GetName() const;		
		TextureType GetType() const;
		


		/* Static texture loading functions */
		// -------------------------------------
        /* Load texture and returns texture ID */
		static unsigned LoadNativeTexture(const std::string& name, GLenum target = GL_TEXTURE_2D, int desiredChannels = 0 );

		/* Generates ID for texture target */
        static unsigned GenID( GLenum target = GL_TEXTURE_2D );
		
		/* Stores raw image properties in RAII struct */				
		static RawImage LoadRawImage( const std::string &name, int desiredChannels = 0 );

		/* Converts number of channels to native enum format */
		static GLint GetFormat( int nChannels );

		/* Binds to array and specifies texture image to native lib */
        static void BindTextureImage( unsigned id, RawImage &rawimage, GLenum target = GL_TEXTURE_2D, bool genmm = true );
        static void BindTextureImage( unsigned id, int width, int height, GLint internalformat,
                                      const stbi_uc *data, GLenum target = GL_TEXTURE_2D, bool genmm = true, GLenum format = 0, 
									  GLint level = 0, GLenum type = GL_UNSIGNED_BYTE );

		/* Set texture parameter */
		template<typename T>
        static void SetTexParam( GLenum pname,		 T param,  GLenum target = GL_TEXTURE_2D );
        /* Set texture parameters */
        template <typename T> 
		static void SetTexParam( GLenum pname, const T* param, GLenum target = GL_TEXTURE_2D );


		/* Texture type parsers */
        // -------------------------------------
		/* str_ must be in format: texture_{type} */
		inline static TextureType TypeFromString(const std::string& str_)
		{
            return static_cast<TextureType>(std::toupper(str_[8]));
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
		std::string m_name;		
		TextureType m_type;
	};


} // Redner


#define TDIFFUSE   Render::TextureType::DIFFUSE
#define TNORMAL    Render::TextureType::NORMAL
#define TSPECULAR  Render::TextureType::SPECULAR
#define THEIGHT    Render::TextureType::HEIGHT
