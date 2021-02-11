#pragma once

#include <string>

namespace Render {	

	class Texture
	{
	public:	

		Texture(std::string name, const std::string& type = "texture_diffuse");

		unsigned GetID();
		std::string GetName();
		std::string GetType();
		
		// inline static const char* textureDir = "Resources\\Textures\\";

	private:
		unsigned m_ID;
		std::string m_Name;
		std::string m_Type;
		
	};

} // Redner