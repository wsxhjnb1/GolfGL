#version 330 core

layout(location=0) in vec3 Position;
layout(location=1) in vec2 TexCoords;
layout(location=2) in vec3 Normal;

out VERT_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	float texH;	
} vertOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	vec4 worldPosition = model * vec4(Position, 1.0f);

	gl_Position = projection * view * worldPosition;	

	vertOut.FragPos = worldPosition.xyz;
	vertOut.Normal = mat3(transpose(inverse(model))) * Normal;

	vertOut.TexCoords = TexCoords;

	vertOut.texH = (Position.y + 25) / 25.0;		
}
