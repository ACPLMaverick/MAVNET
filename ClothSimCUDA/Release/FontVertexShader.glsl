#version 400 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec4 vertexColor;

uniform mat4 WorldViewProj;
out vec4 Vcol;
out vec2 UV;

void main()
{
	vec4 v = vec4(vertexPosition, 1.0f);
	gl_Position = WorldViewProj * v;

	Vcol = vertexColor;
	UV = vertexUV;
}