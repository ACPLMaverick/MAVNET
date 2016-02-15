#version 400 core

#define POWER_CORRECTION 1.0f

in vec4 ProjPos;
in vec4 WorldPos;
in vec4 Vcol;
smooth in vec3 Normal;
in vec2 UV;

out vec4 color;

uniform sampler2D sampler;
uniform vec4 EyeVector;
uniform vec4 LightDir;
uniform vec4 LightDiff;
uniform vec4 LightSpec;
uniform vec4 LightAmb;
// uniform vec4 highlight;
uniform float Gloss;

void main()
{
	vec4 finalColor = Vcol * texture(sampler, UV);
	vec3 normal = normalize(Normal);
	
	float lightPower = clamp(dot((LightDir).xyz, normal), 0.0f, 1.0f);
	finalColor.xyz *= lightPower;
	finalColor.xyz *= LightDiff.xyz;

	vec3 R = normalize(reflect(LightDir.xyz, normalize(normal)));
	float specGloss = max(0.0f, dot(EyeVector.xyz, R));
	vec3 spec = LightSpec.xyz * lightPower * pow(specGloss, Gloss);

	finalColor.xyz += spec + LightAmb.xyz;

	color = finalColor;
}