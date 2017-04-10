#include "../_global/GlobalDefines.hlsli"

cbuffer LightAmbient
{
	float4 gColor;
};

Texture2D TexColor : register(t0);
SamplerState SmpColor : register(s0);

Texture2D TexNormal : register(t1);
SamplerState SmpNormal : register(s1);

Texture2D TexWorldPos : register(t2);
SamplerState SmpWorldPos : register(s2);

Texture2D TexDepth : register(t3);
SamplerState SmpDepth : register(s3);

float4 main(DPixelInput input) : SV_TARGET
{
	float4 color = TexColor.Sample(SmpColor, input.Uv);
	return color * gColor;
}