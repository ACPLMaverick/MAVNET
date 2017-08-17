#include "../_global/GlobalDefines.hlsli"

#define SAMPLE_COUNT 14

cbuffer LightCommon : register(b0)
{
	float4x4 projInverse;
};

cbuffer SimpleSSAO : register(b1)
{
	float4x4 gProj;
	float4 gOffsets[SAMPLE_COUNT];
	float4 gParams;
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
	float4 tInput = TexInput.Sample(SmpInput, input.Uv);
	float depth = TexDepth.Sample(SmpDepth, input.Uv).r;
	float3 normal = TexNormal.Sample(SmpNormal, input.Uv).xyz;
	float3 viewPos = ViewPositionFromDepth(projInverse, input.Uv, depth);
	float maxDist = gParams.x;

	
	//tInput.b = tInput.g = 0.0f;
	return tInput;
}