#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;

in float frag;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 brightColor;

in vec2 worldxz;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	vec3 normal = normalize(Normal);

	vec4 textureColor = mix(texture(texture2, TextCoords), texture(texture1, TextCoords), frag);	

	vec3 res = vec3(textureColor);

	color = vec4(res, 1.0f);

	brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

