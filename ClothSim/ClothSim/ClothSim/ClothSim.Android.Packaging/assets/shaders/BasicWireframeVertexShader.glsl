#version 300 es

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexNormal;
layout(location = 3) in vec4 vertexColor;
layout(location = 4) in vec4 vertexBar;

uniform mat4 WorldViewProj;
uniform mat4 World;
uniform mat4 WorldInvTrans;

out vec4 ProjPos;
out vec4 WorldPos;
out vec4 Vcol;
out vec3 Normal;
out vec2 UV;
out vec3 Bar;

void main()
{
	vec4 v = vertexPosition;
	gl_Position = WorldViewProj * v;

	ProjPos = WorldViewProj * v;
	WorldPos = World * v;

	Vcol = vertexColor;
	UV = vertexUV;
	Normal = normalize(vec3((vec4(vertexNormal.x, vertexNormal.y, vertexNormal.z, 0.0f) * WorldInvTrans)));

	/*
	vec3 b = vec3(0.0f);
	if(gl_VertexID % 3 == 0)
		b = vec3(1.0f, 0.0f, 0.0f);
	if(gl_VertexID % 3 == 1)
		b = vec3(0.0f, 1.0f, 0.0f);
	if(gl_VertexID % 3 == 2)
		b = vec3(0.0f, 0.0f, 1.0f);
	Bar = b;*/
	Bar = vec3(vertexBar.x, vertexBar.y, vertexBar.z);
}