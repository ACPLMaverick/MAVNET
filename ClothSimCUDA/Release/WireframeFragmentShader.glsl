#version 400 core

in vec4 Vcol;

out vec4 color;

void main()
{
	color = vec4(1.0f - Vcol.r, 1.0f - Vcol.g, 1.0f - Vcol.b, 1.0f);
}