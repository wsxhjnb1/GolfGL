#version 330 core
layout (location = 0) in vec4 aVertex;

out vec2 TexCoords;

void main()
{
    TexCoords = aVertex.zw;
    gl_Position = vec4(aVertex.x, aVertex.y, 0.0, 1.0); 
}  