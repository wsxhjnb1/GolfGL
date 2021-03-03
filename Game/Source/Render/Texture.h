#pragma once

#include <string>

namespace Render {	

	
	class Texture
	{
	public:	

		Texture(const std::string& name, TextureType type = TextureType::DIFFUSE);

		auto GetID() -> unsigned;
		auto GetName() -> std::string;		
		auto GetType() -> Render::TextureType;
		
		static auto LoadNativeTexture(const std::string& name
					, Render::TextureType type = Render::TextureType::DIFFUSE) -> unsigned;	

		// string in format "texture_{type}"
		inline static auto TypeFromString(const std::string& type_) -> TextureType
		{
			return (TextureType)std::toupper(type_[8]);
		}

		inline static auto TypeToString(TextureType type) -> std::string
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
		inline static auto TypeToStringN(TextureType type, unsigned N) -> std::string
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
