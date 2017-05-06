#include "../_global/GlobalDefines.hlsli"

cbuffer LightCommon
{
	float4x4 projInverse;
	float4x4 viewInverse;
	float3 gViewPosition;
};

Texture2D TexColor : register(t0);
SamplerState SmpColor : register(s0);

Texture2D TexNormal : register(t1);
SamplerState SmpNormal : register(s1);

Texture2D TexDepth : register(t2);
SamplerState SmpDepth : register(s2);

Texture2D TexInput : register(t3);
SamplerState SmpInput : register(s3);

Texture2D TexBuffer : register(t4);
SamplerState SmpBuffer : register(s4);

float4 main(DPixelInput input) : SV_TARGET
{
	float4 inputSample = TexBuffer.Sample(SmpBuffer, input.Uv);

	return inputSample;
}