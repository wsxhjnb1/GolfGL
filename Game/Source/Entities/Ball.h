#pragma once

#include <Render/Model.h>
#include <Render/Shader.h>

class Ball : public Render::Model
{
public:
	Ball();
	~Ball();

	void Update(float delta) override;
private:

	Render::Shader m_Shader;

	glm::vec3 direction;
	glm::vec3 position;

	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;	
	
	unsigned m_Texture;

	float m_speed;
	bool m_moving = false;
	float m_timeout = 0.f;
};