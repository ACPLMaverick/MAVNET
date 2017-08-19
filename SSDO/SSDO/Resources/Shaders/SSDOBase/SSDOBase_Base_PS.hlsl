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

Texture2D TexRandomVectors : register(t5);
SamplerState SmpRandomVectors : register(s5);

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

float4 main(DPixelInput input) : SV_TARGET
{
	float depth = TexDepth.Sample(SmpDepth, input.Uv).r;
	float4 normalSample = TexNormal.Sample(SmpNormal, input.Uv);
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

	float4 final = float4(0.0f, 0.0f, 0.0f, 1.0f);

	[unroll]
	for (int i = 0; i < SAMPLE_COUNT; ++i)
	{
		// Acquire sample position
		// Reflecting an offset by a random normal will give random but uniformly distributed vectors.
		float3 offset = reflect(gOffsets[i].xyz, randomVec);
		offset = normalize(offset);
		float flip = sign(dot(offset, normal));
		offset = offset * flip;
		float4 samplePos = float4(viewPos + maxDist * offset, 1.0f);
		float3 viewSamplePos = samplePos.xyz;

		// Go from view position to screen position (so the depth buffer could be directly used)
		samplePos = mul(samplePos, gProj);
		samplePos /= samplePos.w;

		// Get sample position from depth buffer
		float sampleDepth = samplePos.z;
		float2 mapUv = (samplePos.xy + 1.0f) * 0.5f;
		mapUv.y = 1.0f - mapUv.y;
		float mapDepth = TexDepth.Sample(SmpDepth, mapUv).r;
		float3 mapViewPos = ViewPositionFromDepth(projInverse, mapUv, mapDepth);

		// Compute occlusion for this sample
		float distZ = mapViewPos.z - viewSamplePos.z;
		float3 sampleDirection = normalize(viewSamplePos - mapViewPos);
		// Directional scale factor for occlusion (based on angle between normal and sample point direction)
		float dp = max(dot(normal, offset), 0.0f);

		// Lit from this sample
		float4 lit = float4(0.0f, 0.0f, 0.0f, 1.0f);
		PhongBlinn(gLightColor,
			1.0f,
			normalize(viewSamplePos - viewPos),
			gLightDirection,
			-normalize(viewSamplePos),
			pData,
			lit);
		
		// take this sample into consideration or not, depending whether it is above surface or not.
		float litFactor = sign(max(distZ, 0.0f));
		//litFactor *= max(dot(normal, sampleDirection), 0.0f);
		litFactor *= (dp);
		lit *= litFactor;

		//if (abs(distZ) > maxDist)
		//	lit = float4(1.0f, 1.0f, 1.0f, 1.0f);

		final += saturate(lit);
	}


	//final = final / litCounter;

	final = pow(final, powFactor);
	final = saturate(final);
	//final.y = final.z = final.x;

	return final;
}