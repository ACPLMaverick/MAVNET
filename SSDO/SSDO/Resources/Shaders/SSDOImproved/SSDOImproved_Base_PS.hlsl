#include "../_global/GlobalDefines.hlsli"
#include "../_global/Lighting.hlsli"

#define SAMPLE_COUNT 14

cbuffer LightCommon : register(b0)
{
	float4x4 projInverse;
};

cbuffer SSDOBase : register(b1)
{
	float4x4 gProj;
	float4 gOffsets[SAMPLE_COUNT];
	float4 gParams;
	float4 gLightColor;
	float3 gLightDirection;
};

struct PixelOutput
{
	float4 final : SV_TARGET0;
};

BASE_TEXTURES;

Texture2D TexInput : register(t3);
SamplerState SmpInput : register(s3);

Texture2D TexBuffer : register(t4);
SamplerState SmpBuffer : register(s4);

Texture2D TexBufferB : register(t5);
SamplerState SmpBufferB : register(s5);

Texture2D TexRandomVectors : register(t6);
SamplerState SmpRandomVectors : register(s6);

float Occlusion(float distZ)
{
	const float EPSILON = gParams.z;
	const float FADE_START = gParams.y;
	const float FADE_END = gParams.x;

	float occlusion = 0.0f;

	if (distZ > EPSILON)
	{
		float fadeLength = FADE_END - FADE_START;
		occlusion = saturate((FADE_END - distZ) / fadeLength);
	}

	return occlusion;
}

PixelOutput main(DPixelInput input)
{
	float4 normalSample = TexNormalDepth.Sample(SmpNormalDepth, input.Uv);
	float depth = normalSample.w;
	float3 normal = normalSample.xyz;
	float3 viewPos = ViewPositionFromDepth(projInverse, input.Uv, depth);
	float3 randomVec = TexRandomVectors.Sample(SmpRandomVectors, input.Uv).xyz;
	const float maxDist = gParams.x;
	const float powFactor = gParams.w;

	float occlusionCounter = 0.0f;
	float occlusionDivisor = SAMPLE_COUNT;

	MaterialData pData;
	pData.colBase = float4(1.0f, 1.0f, 1.0f, 1.0f);
	pData.colSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	pData.gloss = normalSample.w;

	PixelOutput output;

	output.final = float4(1.0f, 0.0f, 0.0f, 1.0f);
	output.final *= TexInput.Sample(SmpInput, input.Uv);

	return output;
}