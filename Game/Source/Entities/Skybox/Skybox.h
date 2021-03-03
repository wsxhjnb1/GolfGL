#pragma once

#include <Entities/Entity.h>

namespace Entities {
		
	class Skybox : public Entity
	{
	public:
		Skybox();
		~Skybox();

		void Update(float delta) override;
	private:
		unsigned m_VAO, m_VBO;
		

		auto m_LoadCubemap() -> unsigned;
		inline void updatePV() override
		{
			projection = glm::perspective(glm::radians(Camera::GetCamera().GetCameraZoom())
				, (float)WindowData::width / (float)WindowData::height, 0.1f, 100.0f);

			view = glm::mat4(glm::mat3(Camera::GetCamera().LookAt()));			
		}
	};
	
}