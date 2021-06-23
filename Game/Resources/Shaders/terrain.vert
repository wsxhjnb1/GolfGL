#version 330 core

// Attributes
layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 TexCoords;
layout(location=3) in vec3 Tangent;
layout(location=4) in vec3 Bitangent;

// Output
out VERT_OUT
{
	vec2 TexCoords;	
	float texH;
	vec3 FragPos;
	vec3 LightPos;
	vec3 ViewPos;
} vertOut;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec4 worldPosition = model * vec4(Position, 1.f);

	gl_Position = projection * view * worldPosition;	

	vertOut.FragPos = worldPosition.xyz;
	vertOut.TexCoords = TexCoords;
	
	vec3 T = normalize(vec3(model * vec4(Tangent, 0.f)));
	vec3 B = normalize(vec3(model * vec4(Bitangent, 0.f)));
	vec3 N = normalize(vec3(model * vec4(Normal, 0.f)));
	mat3 TBN = transpose(mat3(T,B,N));
	
	vertOut.FragPos = TBN * vertOut.FragPos;
	vertOut.LightPos = TBN * lightPos;
	vertOut.ViewPos = TBN * viewPos;

	vertOut.texH = (Position.y + 25.f) / 25.f;
}
