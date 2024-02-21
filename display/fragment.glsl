#version 440 core

out vec4 f_color;

in vec2 g_texCoord;

uniform sampler2D smp;

void main()
{
    f_color = texture(smp, g_texCoord);
    // f_color = vec4(g_texCoord, 1.0, 1.0);
}