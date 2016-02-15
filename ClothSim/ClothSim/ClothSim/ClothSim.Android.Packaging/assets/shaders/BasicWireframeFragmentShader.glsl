#version 300 es
#extension GL_OES_standard_derivatives : enable 

in vec4 ProjPos;
in vec4 WorldPos;
in vec3 Normal;
in vec2 UV;
in vec4 Vcol;
in vec3 Bar;

uniform sampler2D sampler;
uniform vec4 EyeVector;
uniform vec4 LightDir;
uniform vec4 LightDiff;
uniform vec4 LightSpec;
uniform vec4 LightAmb;
uniform float Gloss;
uniform float Specular;

out vec4 color;

void main()
{
	vec3 d = fwidth(Bar) + vec3(0.05f);
    vec3 a3 = smoothstep(vec3(0.0f), d*1.5, Bar);
    float edgeFactor = min(min(a3.x, a3.y), a3.z);
    edgeFactor = clamp(edgeFactor, 0.0f, 1.0f);
    //edgeFactor = 1.0f - edgeFactor;

    /*
    float edgeFactor = 0.0f;
    if(any(lessThan(Bar, vec3(0.04))))
    {
    	edgeFactor = 1.0f;
	}
    */

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

	color = vec4(mix(1.0f - Vcol.r, finalColor.r, edgeFactor), 
        mix(1.0f - Vcol.g, finalColor.g, edgeFactor), 
        mix(1.0f - Vcol.b, finalColor.b, edgeFactor), Vcol.a);
}