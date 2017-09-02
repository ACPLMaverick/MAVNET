#include "../_global/GlobalDefines.hlsli"
#include "../_global/Lighting.hlsli"

#define SAMPLE_COUNT 14

cbuffer LightCommon : register(b0)
{
	float4x4 gProjInverse;
};

cbuffer SSDOBase : register(b1)
{
	float4 gLightColor;
	float3 gLightDirection;
	float4 gSatDimensions;
	float gSampleBoxHalfSize;
	float gSampleBoxHalfDiagonal;
	float gOcclusionFalloff;
	float gPowFactor;
};

struct PixelOutput
{
	float4 final : SV_TARGET0;
};

BASE_TEXTURES;

Texture2D TexInput : register(t3);
SamplerState SmpInput : register(s3);

Texture2D SatNormalDepth : register(t4);
SamplerState SmpSatNormalDepth : register(s4);

Texture2D SatColor : register(t5);
SamplerState SmpSatColor : register(s5);

void ApplyOcclusionFaloff(float diffZ, inout float occlusion)
{
	occlusion = max(occlusion, 0.0f);
	occlusion *= smoothstep(-(1.0f + gOcclusionFalloff), -1.0f, -occlusion);
	occlusion *= step(0.0001f, diffZ);
}

// Will probably be used in the future when adaptive layering is implemented.
void SampleDepth(float2 uv, out float depth)
{
	depth = 1.0f - SatNormalDepth.Sample(SmpSatNormalDepth, uv).w;
}

void GetAverageValues(float2 uv, float depth, out float3 avgColor, out float3 avgNormal, out float avgDepth)
{
	const uint sampleCount = 4;
	const float boxHalfDiagonal = gSampleBoxHalfDiagonal * depth;
	const float2 uvSamples[sampleCount] =
	{
		saturate(float2(uv.x - boxHalfDiagonal, uv.y - boxHalfDiagonal)),
		saturate(float2(uv.x + boxHalfDiagonal, uv.y - boxHalfDiagonal)),
		saturate(float2(uv.x + boxHalfDiagonal, uv.y + boxHalfDiagonal)),
		saturate(float2(uv.x - boxHalfDiagonal, uv.y + boxHalfDiagonal))
	};	// TL, TR, BR, BL

	float4 normalsDepths[sampleCount];
	float3 colors[sampleCount];

	[unroll]
	for (uint i = 0; i < sampleCount; ++i)
	{
		normalsDepths[i] = SatNormalDepth.Sample(SmpSatNormalDepth, uvSamples[i]);
	}
	[unroll]
	for (i = 0; i < sampleCount; ++i)
	{
		colors[i] = SatNormalDepth.Sample(SmpSatNormalDepth, uvSamples[i]).rgb;
	}

	// SAT: (BR - BL - TR + TL) / area
	const float areaRec = 1.0f / ( (uvSamples[2].x - uvSamples[3].x) * gSatDimensions.x * (uvSamples[2].y - uvSamples[1].y) * gSatDimensions.y );

	const float4 avgNormalDepth = (normalsDepths[2] - normalsDepths[3] - normalsDepths[1] + normalsDepths[0]) * areaRec;
	avgNormal = normalize(avgNormalDepth.xyz);
	avgDepth = avgNormalDepth.w;

	avgColor = (colors[2] - colors[3] - colors[1] + colors[0]) * areaRec;
}

float GetOcclusion(float3 pixelNormal, float2 uv, float pixelDepth, float avgDepth)
{
	const float3 pxViewPos = ViewPositionFromDepth(gProjInverse, uv, pixelDepth);
	const float3 avgViewPos = ViewPositionFromDepth(gProjInverse, uv, avgDepth);
	const float diffZ = abs(pixelDepth - avgDepth);

	const float3 dirToAvg = avgViewPos - pxViewPos;

	float occlusion = dot(dirToAvg, pixelNormal);
	occlusion /= gSampleBoxHalfSize;
	ApplyOcclusionFaloff(diffZ, occlusion);
	occlusion = pow(occlusion, gPowFactor);

	return occlusion;
}

PixelOutput main(DPixelInput input)
{
	float4 normalSample = TexNormalDepth.Sample(SmpNormalDepth, input.Uv);
	float depth = normalSample.w;
	float3 normal = normalSample.xyz;

	float3 avgColor, avgNormal;
	float avgDepth;
	GetAverageValues(input.Uv, depth, avgColor, avgNormal, avgDepth);


	float occlusion = GetOcclusion(normal, input.Uv, depth, avgDepth);


	MaterialData pData;
	pData.colBase = float4(1.0f, 1.0f, 1.0f, 1.0f);
	pData.colSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	pData.gloss = normalSample.w;

	PixelOutput output;

	occlusion = 1.0f - occlusion;
	//output.final = TexInput.Sample(SmpInput, input.Uv) * occlusion.rrrr;
	output.final = occlusion.rrrr;

	return output;
}