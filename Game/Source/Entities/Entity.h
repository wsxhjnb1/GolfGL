#pragma once
#include <precomp.h>

#include <Render/Shader.h>
#include <Render/Texture.h>

namespace Entities {
	class Entity
	{
	public:
		virtual void Update(float delta) = 0;

		virtual ~Entity() { glDeleteTextures(1, &texture); }

	protected:
		Entity(const std::string& shaderName)
			: shader(shaderName), texture(0)
		{}

		Entity(const std::string& shaderName, const std::string& texturePath)
			: shader(shaderName)
			, texture(Render::Texture::LoadNativeTexture(texturePath))
		{}

		Entity(const std::string& shaderName, const unsigned textureID)
			: shader(shaderName)
			, texture(textureID)
		{}

		glm::mat4 projection{ 1.f };
		glm::mat4 view{ 1.f };
		glm::mat4 model{ 1.f };
		glm::vec3 position{ 0.f };

		Render::Shader shader;
		unsigned texture;		


		// inlined methods
		inline virtual void updatePV()
		{
			projection = glm::perspective(glm::radians(CAMERA.GetCameraZoom())
				, (float)WindowData::width / (float)WindowData::height, 0.1f, 100.0f);

			view = CAMERA.LookAt();
		}

		inline void setUniformPVM()
		{
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
			shader.setMat4("model", model);
		}
	};
}