#pragma once

#include <Model/Model.h>
#include <Entities/Entity.h>

namespace Entities {

	class Ball : public Render::Model
				, public Entity
	{
	public:
		Ball();
		~Ball();

		void Update(float delta) override;
	private:
		glm::vec3 direction;
		glm::vec3 position;

		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;		

		float m_speed;
		bool m_moving = false;
		float m_timeout = 0.f;
	};

}