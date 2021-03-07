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
	class Texture
	{
	public:	

		Texture(const std::string& name, TextureType type = TextureType::DIFFUSE);

		unsigned GetID();
		std::string GetName();		
		Render::TextureType GetType();
		
		static unsigned LoadNativeTexture(const std::string& name
					, Render::TextureType type = Render::TextureType::DIFFUSE);	

		// string in format "texture_{type}"
		inline static TextureType TypeFromString(const std::string& type_)
		{
			return (TextureType)std::toupper(type_[8]);
		}

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

		/* string in format "texture_{_type_}N" where N is 
		*  ordinary number of texture 
		*/
		inline static std::string TypeToStringN(TextureType type, unsigned N)
		{
			return TypeToString(type) + std::to_string(N);
		}

	private:
		unsigned m_ID;
		std::string m_Name;		
		TextureType m_Type;		
	};
	
} // Redner


#define TDIFFUSE   Render::TextureType::DIFFUSE
#define TNORMAL    Render::TextureType::NORMAL
#define TSPECULAR  Render::TextureType::SPECULAR
#define THEIGHT    Render::TextureType::HEIGHT
