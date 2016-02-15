#version 400 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec4 vertexColor;

uniform mat4 WorldViewProj;
uniform mat4 World;
uniform mat4 WorldInvTrans;

out vec4 ProjPos;
out vec4 WorldPos;
out vec4 Vcol;
smooth out vec3 Normal;
out vec2 UV;

void main()
{
	vec4 v = vec4(vertexPosition, 1.0f);
	gl_Position = WorldViewProj * v;

	ProjPos = v * WorldViewProj;
	WorldPos = v * World;

	Vcol = vertexColor;
	UV = vertexUV;
	Normal = normalize(vec3((vec4(vertexNormal, 1.0f) * WorldInvTrans)));
}