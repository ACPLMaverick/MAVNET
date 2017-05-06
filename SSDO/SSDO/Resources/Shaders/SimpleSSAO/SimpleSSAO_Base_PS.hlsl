#include "../_global/GlobalDefines.hlsli"

cbuffer LightCommon : register(b0)
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

float4 main(DPixelInput input) : SV_TARGET
{
	//float4 inputSample = TexDepth.Sample(SmpDepth, input.Uv);
	float4 inputSample = TexInput.Sample(SmpInput, input.Uv);

	return inputSample;
}