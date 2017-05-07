#include "../_global/GlobalDefines.hlsli"

cbuffer LightCommon
{
	float4x4 projInverse;
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
	float4 inputSample = TexInput.Sample(SmpInput, input.Uv);
	inputSample.r = (inputSample.r * 0.393f) + (inputSample.g * 0.769f) + (inputSample.b * 0.189f);
	inputSample.g = (inputSample.r * 0.349f) + (inputSample.g * 0.686f) + (inputSample.b * 0.168f);
	inputSample.b = (inputSample.r * 0.272f) + (inputSample.g * 0.534f) + (inputSample.b * 0.131f);
	return inputSample;
}