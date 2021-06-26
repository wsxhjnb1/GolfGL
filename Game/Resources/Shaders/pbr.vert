#version 330 core
/* Attributes */
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

/* Output */
out VERT_OUT
{
    vec2 TexCoords;
    vec3 WorldPos;
    vec3 Normal;
} vertOut;

uniform mat4 PV;
uniform mat4 model;

void main()
{
    vertOut.TexCoords = TexCoords;
    vertOut.WorldPos = vec3(model * vec4(Position, 1.0));
    vertOut.Normal = mat3(model) * Normal;

    gl_Position =  PV * vec4(vertOut.WorldPos, 1.0);
}