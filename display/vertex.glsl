#version 440 core

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 texCoord;

out vec2 g_texCoord;

void main() 
{
    gl_Position = vec4(vertexPosition, 0.0, 1.0);
    g_texCoord = texCoord;
}
