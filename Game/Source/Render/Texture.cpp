#include <precomp.h>

#include "Texture.h"

#include <stb_image.h>

Render::Texture::Texture(std::string name, const std::string& type)
	: m_Name(name.substr( name.find_last_of('/') + 1)), m_Type(type)
{	
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	


	// Calculating full texture path
	// name.insert(0, textureDir);

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

		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		LOG_ERROR("Failed to load texture\nTexture: {}", name);
		stbi_image_free(data);
		throw std::exception("Failed to load image");
	}

	
#ifdef _DEBUG
	LOG_INFO("Loaded texture {}", m_Name);
#endif // DEBUG	

	stbi_image_free(data);
}

unsigned Render::Texture::GetID()
{
	return m_ID;
}

std::string Render::Texture::GetName()
{
	return m_Name;
}

std::string Render::Texture::GetType()
{
	return m_Type;
}

