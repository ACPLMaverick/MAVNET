#version 300 es

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexNormal;
layout(location = 3) in vec4 vertexColor;

uniform mat4 WorldViewProj;
uniform mat4 World;
out vec4 Vcol;
out vec2 UV;

void main()
{
	vec4 v = vertexPosition;
	gl_Position = WorldViewProj * v;

	Vcol = vertexColor;
	vec2 suv = vertexUV;
	suv = suv * 2.0f - 1.0f;
	vec4 vu = vec4(suv, 0.0f, 1.0f);
	vu = World * vu;
	UV = ((vec2(vu.x, vu.y) + 1.0f) / 2.0f);
}