#include "DeferredDrawShader_H.hlsli"

Texture2D TexColor;
SamplerState SmpColor;

Texture2D TexNormal;
SamplerState SmpNormal;

Texture2D TexDepth;
SamplerState SmpDepth;

float4 main(PixelInput input) : SV_TARGET
{
	float4 color = TexColor.Sample(SmpColor, input.Uv);
	float3 normal = normalize(TexNormal.Sample(SmpNormal, input.Uv).xyz);
	float depth = TexDepth.Sample(SmpDepth, input.Uv).x;

	return float4(normal, 1.0f);
}