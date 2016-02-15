#version 300 es

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexNormal;
layout(location = 3) in vec4 vertexColor;

uniform mat4 WorldViewProj;
uniform mat4 World;
uniform mat4 WorldInvTrans;

out vec4 ProjPos;
out vec4 WorldPos;
out vec4 Vcol;
out vec3 Normal;
out vec2 UV;

void main()
{
	vec4 v = vertexPosition;
	gl_Position = WorldViewProj * v;

	ProjPos = WorldViewProj * v;
	WorldPos = World * v;

	Vcol = vertexColor;
	UV = vertexUV;
	Normal = normalize(vec3((vec4(vertexNormal.x, vertexNormal.y, vertexNormal.z, 0.0f) * WorldInvTrans)));
}