#include <precomp.h>

#include "Texture.h"



Render::Texture::Texture(const std::string& name, Render::TextureType type)
	: m_Name(name.substr( name.find_last_of('/') + 1)), m_Type(type)
{	
	m_ID = LoadNativeTexture(name, type);
}

unsigned Render::Texture::GetID()
{
	return m_ID;
}

std::string Render::Texture::GetName()
{
	return m_Name;
}

Render::TextureType Render::Texture::GetType()
{
	return m_Type;
}

unsigned Render::Texture::LoadNativeTexture(const std::string& name, Render::TextureType type)
{
	unsigned id;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	

	int width, height, nrChannels;

	unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum format = GL_RED;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		LOG_INFO("Loaded {} {}", type, name);
	}
	else
	{
		LOG_ERROR("Failed to load texture\nTexture: {}", name);
		id = 0;
	}



	stbi_image_free(data);

	return id;
}

