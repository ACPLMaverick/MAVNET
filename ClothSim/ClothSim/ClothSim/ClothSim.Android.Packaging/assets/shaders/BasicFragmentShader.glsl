#version 300 es

#define POWER_CORRECTION 1.0f

in vec4 ProjPos;
in vec4 WorldPos;
in vec4 Vcol;
in vec3 Normal;
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
uniform float Specular;

void main()
{
	vec4 finalColor = Vcol * texture(sampler, UV);
	vec3 normal = normalize(Normal);
	vec3 lDir = vec3(-LightDir.x, -LightDir.y, LightDir.z);
	
	vec3 surfaceToCamera = normalize(EyeVector.xyz - WorldPos.xyz);
	surfaceToCamera.z = -surfaceToCamera.z;
	vec3 H = normalize(surfaceToCamera + normalize(lDir));
	float intensity = max(dot(lDir, normal), 0.0f);

	finalColor.xyz *= intensity;
	finalColor.xyz *= LightDiff.xyz;
	
	vec3 spec = pow(max(0.0000001f, dot(H, normal)), Gloss) * LightSpec.xyz * length(LightSpec.xyz) * intensity * max(Specular, 0.0f);

	finalColor.xyz += spec + LightAmb.xyz;

	color = finalColor;
}