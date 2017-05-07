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
	float depth = TexDepth.Sample(SmpDepth, input.Uv).r;
	float3 normal = TexNormal.Sample(SmpNormal, input.Uv).xyz;
	float3 viewPos = ViewPositionFromDepth(projInverse, input.Uv, depth);
	float maxDist = gParams.x;

	float occlusionCounter = 0.0f;
	float occlusionDivisor = SAMPLE_COUNT;

	[unroll]
	for (int i = 0; i < SAMPLE_COUNT; ++i)
	{
		float3 offset = gOffsets[i].xyz;
		float3 normalizedOffset = normalize(offset);
		float flip = sign(dot(normalizedOffset, normal));
		float4 samplePos = float4(viewPos + maxDist * offset, 1.0f);
		samplePos = mul(samplePos, gProj);
		samplePos /= samplePos.w;

		float sampleDepth = samplePos.z;
		float2 mapUv = (samplePos.xy + 1.0f) * 0.5f;
		mapUv.y = 1.0f - mapUv.y;
		float mapDepth = TexDepth.Sample(SmpDepth, mapUv).r;
		//float3 mapWorldPos = WorldPositionFromDepth(projInverse, viewInverse, mapUv, mapDepth);

		//occlusionCounter += min(max(sampleDepth - mapDepth, 0.0f), maxDist) / maxDist;
		occlusionCounter += max(sampleDepth - mapDepth, 0.0f) * 10.0f;

		//float3 sampleDir = normalize(gOffsets[i]);
	}

	float occlusion = 1.0f - (occlusionCounter / occlusionDivisor);
	occlusion = pow(occlusion, 4.0f);

	return float4(occlusion, occlusion, occlusion, 1.0f);
}