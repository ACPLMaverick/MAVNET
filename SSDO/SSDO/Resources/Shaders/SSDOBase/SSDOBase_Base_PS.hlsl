#include "../_global/GlobalDefines.hlsli"

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

float3 ColorFactor(float2 mapUv, float distZ)
{
	const float FADE_END = gParams.x;

	// Get color of sample
	float3 mapColor = TexColor.Sample(SmpColor, mapUv).rgb;
	// Compute scale factor: 1 if this sample is above surface, 0 if below
	float colorScale = sign(max(distZ, 0.0f));

	if (distZ > FADE_END)
		colorScale = 0.0f;
	
	return mapColor * colorScale;
}

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
	float3 normal = TexNormal.Sample(SmpNormal, input.Uv).xyz;
	float3 viewPos = ViewPositionFromDepth(projInverse, input.Uv, depth);
	float3 randomVec = TexRandomVectors.Sample(SmpRandomVectors, input.Uv).xyz;
	const float maxDist = gParams.x;
	const float powFactor = gParams.w;
	float3 colorFactor = float3(0.0f, 0.0f, 0.0f);

	float occlusionCounter = 0.0f;
	float occlusionDivisor = SAMPLE_COUNT;

	[unroll]
	for (int i = 0; i < SAMPLE_COUNT; ++i)
	{
		// Acquire sample position
		// Reflecting an offset by a random normal will give random but uniformly distributed vectors.
		float3 offset = reflect(gOffsets[i].xyz, randomVec);
		float3 normalizedOffset = normalize(offset);
		float flip = sign(dot(normalizedOffset, normal));
		float4 samplePos = float4(viewPos + maxDist * offset, 1.0f);

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
		float distZ = viewPos.z - mapViewPos.z;
		// Directional scale factor for occlusion (based on angle between normal and sample point direction)
		float dp = max(dot(normal, normalize(mapViewPos - viewPos)), 0.0f);
		occlusionCounter += dp * Occlusion(distZ);

		// Compute color factor for sample and add mix with global variable
		colorFactor += ColorFactor(mapUv, distZ);
	}

	float occlusion = 1.0f - (1.0f * occlusionCounter / occlusionDivisor);
	occlusion = pow(occlusion, powFactor);

	// Mix occlusion with color factor
	colorFactor = saturate(normalize(colorFactor));
	float4 final = saturate(float4(occlusion, occlusion, occlusion, 1.0f) + float4(colorFactor.x, colorFactor.y, colorFactor.z, 0.0f));

	return final;
}