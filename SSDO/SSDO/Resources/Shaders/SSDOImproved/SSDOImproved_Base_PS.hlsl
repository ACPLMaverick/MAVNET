#include "../_global/GlobalDefines.hlsli"
#include "../_global/Lighting.hlsli"
#include "../SAT/SATUtility.hlsli"

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
	float gOcclusionPower;
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

// Layered SATs
Texture2D SatNormalDepthLayer1 : register(t6);
SamplerState SmpSatNormalDepthLayer1 : register(s6);

Texture2D SatNormalDepthLayer10 : register(t7);
SamplerState SmpSatNormalDepthLayer10 : register(s7);

Texture2D SatNormalDepthLayer20 : register(t8);
SamplerState SmpSatNormalDepthLayer20 : register(s8);

Texture2D SatIndices : register(t9);
SamplerState SmpSatIndices : register(s9);

Texture2D DbgIndices : register(t10);
SamplerState SmpDbgIndices : register(s10);


// FUNCTIONS

float3 Hue(in float h)
{
	float r = abs(h * 6.0f - 3.0f) - 1.0f;
	float g = 2.0f - abs(h * 6.0f - 2.0f);
	float b = 2.0f - abs(h * 6.0f - 4.0f);
	return saturate(float3(r, g, b));
}

float3 HSVtoRGB(in float3 hsv)
{
	return ((Hue(hsv.x) - 1.0f) * hsv.y + 1.0f) * hsv.z;
}

float3 RGBtoHSV(in float3 rgb)
{
	float3 hsv = 0.0f;
	hsv.z = max(rgb.r, max(rgb.g, rgb.b));
	float m = min(rgb.r, min(rgb.g, rgb.b));
	float c = hsv.z - m;

	if (c != 0.0f)
	{
		hsv.y = c / hsv.z;
		float3 delta = (hsv.z - rgb) / c;
		delta.rgb -= delta.brg;
		delta.rg += float2(2.0f, 4.0f);
		if (rgb.r >= hsv.z)
			hsv.x = delta.b;
		else if (rgb.g >= hsv.z)
			hsv.x = delta.r;
		else
			hsv.x = delta.g;

		hsv.x = frac(hsv.x / 6.0f);
	}

	return hsv;
}

void ApplyOcclusionFaloff(in float diffZ, inout float occlusion)
{
	occlusion = max(occlusion, 0.0f);
	occlusion *= 1.0f - smoothstep(1.0f, 1.0f + gOcclusionFalloff, occlusion);
	occlusion *= 1.0f - smoothstep(0.0f, gOcclusionFalloff, diffZ);
	occlusion *= step(0.0001f, diffZ);
}

// Will probably be used in the future when adaptive layering is implemented.
void SampleDepth(in float2 uv, out float depth)
{
	depth = 1.0f - SatNormalDepth.Sample(SmpSatNormalDepth, uv).w;
}

/*
void GetAverageValues(in const float2 uv, in const float depth, in const float colorSampleMultiplier,
	out float3 avgColor, out float3 avgNormal, out float3 avgViewPos, out float avgDepth)
{

	const uint sampleCount = 4;
	const float boxHalfSize = gSampleBoxHalfSize;
	const float boxHalfSizeColor = boxHalfSize * colorSampleMultiplier;

	float2 samplePoints[4];
	float2 samplePointsColor[4];
	float areaRec, areaRecColor;

	GenerateSamplePointsAndArea(gSatDimensions, boxHalfSize, uv, depth, samplePoints, areaRec);
	GenerateSamplePointsAndArea(gSatDimensions, boxHalfSizeColor, uv, depth, samplePointsColor, areaRecColor);

	float4 avgNormalDepth, avgColor4;
	
	CalculateAverage(gSatDimensions, SatNormalDepth, SmpSatNormalDepth, samplePoints, areaRec, avgNormalDepth);
	CalculateAverage(gSatDimensions, SatColor, SmpSatColor, samplePointsColor, areaRecColor, avgColor4);

	avgNormal = normalize(avgNormalDepth.xyz);
	avgDepth = avgNormalDepth.w;
	avgViewPos = ViewPositionFromDepth(gProjInverse, uv, avgDepth);

	avgColor = avgColor4.rgb;
}
*/

void GetAverageValues(in const float2 uv, in const float depth, in const float colorSampleMultiplier,
	out float3 avgColors[4], out float3 avgNormals[4], out float3 avgViewPoses[4], out float avgDepths[4], out float4 indexSums)
{

	const uint sampleCount = 4;
	const float boxHalfSize = gSampleBoxHalfSize;
	const float boxHalfSizeColor = boxHalfSize * colorSampleMultiplier;

	float2 samplePoints[4];
	float2 samplePointsColor[4];
	float areaRec, areaRecColor;

	GenerateSamplePointsAndArea(gSatDimensions, boxHalfSize, uv, depth, samplePoints, areaRec);
	GenerateSamplePointsAndArea(gSatDimensions, boxHalfSizeColor, uv, depth, samplePointsColor, areaRecColor);

	float4 avgNormalDepths[4];
	float4 avgColors4[4];
	float4 avgColor4;
	float4 avgNormalDepth;

	CalculateAverage(gSatDimensions, SatNormalDepthLayer10, SmpSatNormalDepthLayer10, samplePoints, areaRec, avgNormalDepths[0]);
	CalculateAverageDifferential(gSatDimensions, SatNormalDepthLayer10, SatNormalDepthLayer1,
		SmpSatNormalDepthLayer10, SmpSatNormalDepthLayer1, samplePoints, areaRec, avgNormalDepths[1]);
	CalculateAverage(gSatDimensions, SatNormalDepthLayer20, SmpSatNormalDepthLayer20, samplePoints, areaRec, avgNormalDepths[2]);
	CalculateAverageDifferentialGrandparent(gSatDimensions, SatNormalDepthLayer20, SatNormalDepthLayer1, SatNormalDepth,
		SmpSatNormalDepthLayer20, SmpSatNormalDepthLayer1, SmpSatNormalDepth, samplePoints, areaRec, avgNormalDepths[3]);

	CalculateSum(gSatDimensions, SatIndices, SmpSatIndices, samplePoints, indexSums);
	
	CalculateAverage(gSatDimensions, SatColor, SmpSatColor, samplePointsColor, areaRecColor, avgColor4);
	CalculateAverage(gSatDimensions, SatNormalDepth, SmpSatNormalDepth, samplePoints, areaRec, avgNormalDepth);

	[unroll]
	for (uint i = 0; i < 4; ++i)
	{
		avgNormals[i] = avgNormalDepths[i].xyz;
		if (length(avgNormals[i]) > 0.001f)
		{
			avgNormals[i] = normalize(avgNormals[i]);
		}
		avgDepths[i] = avgNormalDepths[i].w;
		avgViewPoses[i] = ViewPositionFromDepth(gProjInverse, uv, avgDepths[i]);
	}
}

float GetOcclusion(in const float3 avgNormal, in const float3 avgViewPos, in const float3 pxViewPos,
	in const float2 uv, in const float pixelDepth, in const float avgDepth)
{
	const float diffZ = abs(pixelDepth - avgDepth);

	const float3 dirToAvg = avgViewPos - pxViewPos;

	// Non normal-sensitive method.
	//float zb = pixelDepth;
	//float zt = pixelDepth * (1.0f + gSampleBoxHalfSize);
	//float occlusion = (zb - avgDepth) / (zb - zt);

	float occlusion = max(dot(dirToAvg, avgNormal), 0.0f);

	occlusion /= gSampleBoxHalfSize;
	ApplyOcclusionFaloff(diffZ, occlusion);
	occlusion = pow(occlusion, gPowFactor);

	return occlusion;
}

PixelOutput main(in DPixelInput input)
{
	const float colorSampleBoxMultiplier = 1.5f;

	const float4 color = TexColor.Sample(SmpColor, input.Uv);
	const float4 normalSample = TexNormalDepth.Sample(SmpNormalDepth, input.Uv);
	const float depth = normalSample.w;
	const float3 normal = normalSample.xyz;
	const float3 viewPos = ViewPositionFromDepth(gProjInverse, input.Uv, depth);

	/*
	// old pipeline
	float3 avgColor, avgNormal, avgViewPos;
	float avgDepth;
	GetAverageValues(input.Uv, depth, colorSampleBoxMultiplier, avgColor, avgNormal, avgViewPos, avgDepth);

	float occlusion = GetOcclusion(avgNormal, avgViewPos, viewPos, input.Uv, depth, avgDepth);
	*/

	const uint layerCount = 4;
	float3 avgColors[layerCount], avgNormals[layerCount], avgViewPoses[layerCount];
	float avgDepths[layerCount];
	float4 indexSums;
	float3 avgNormal = 0.0f, avgColor = 0.0f;
	float4 dbg;

	GetAverageValues(input.Uv, depth, colorSampleBoxMultiplier, avgColors, avgNormals, avgViewPoses, avgDepths, indexSums);

	float occlusions[layerCount];
	
	[unroll]
	for (uint i = 0; i < layerCount; ++i)
	{
		occlusions[i] = GetOcclusion(avgNormals[i], avgViewPoses[i], viewPos, input.Uv, depth, avgDepths[i]);
		occlusions[i] *= indexSums[i];
	}
	
	float occlusion = (occlusions[0] + occlusions[1] + occlusions[2] + occlusions[3]) / 
		(indexSums[0] + indexSums[1] + indexSums[2] + indexSums[3]);

	float4 avgDepth = (avgDepths[0] * indexSums[0] + avgDepths[1] * indexSums[1] + avgDepths[2] * indexSums[2] + avgDepths[3] * indexSums[3]) /
		(indexSums[0] + indexSums[1] + indexSums[2] + indexSums[3]);

	MaterialData pData;
	pData.colBase = float4(1.0f, 1.0f, 1.0f, 1.0f);
	pData.colSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	pData.gloss = 50.0f;

	float4 smpColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	const float visibility = 1.0f - occlusion;
	float directionalFactor = saturate(pow((1.0f - max(dot(avgNormal, gLightDirection), 0.0f)), 5.5f));
	float finalLerpValue = occlusion * directionalFactor * gOcclusionPower;
	smpColor = visibility * float4(normalize(gLightColor.xyz), 1.0f);


	// Indirecity.
	// Basically a difference between pixel color and average color is calculated.
	// Then it is multiplied by indirect factor, calculated totally not similar to SSDO, 
	// but with average normal and average position used.
	float3 indirect = avgColor;
	indirect = color.xyz - avgColor;
	indirect = saturate(indirect);

	// FAST VERSION
	//indirect = (1.0f - indirect) * saturate(length(indirect)) * avgColor * gLightColor.xyz;

	// ACCURATE VERSION
	indirect = RGBtoHSV(indirect);
	indirect.r = 0.785398f - indirect.r;
	indirect = HSVtoRGB(indirect);
	indirect *= avgColor * gLightColor.xyz * pow(1.0f - directionalFactor, 3.0f) * (1.0f - occlusion);


	PixelOutput output;
	const float4 colorInput = TexInput.Sample(SmpInput, input.Uv);
	smpColor *= colorInput;

	float4 indSample = DbgIndices.Sample(SmpDbgIndices, input.Uv);

	//output.final = lerp(colorInput, smpColor, finalLerpValue) /*+ float4(indirect.xyz, 1.0f)*/;
	output.final = occlusion.rrrr * saturate(indSample + indSample.aaaa);
	//output.final = float4(length(avgNormals[0]), length(avgNormals[3]), length(avgNormals[2]), length(avgNormals[3]));
	//output.final = indSample + indSample.aaaa;
	//output.final = abs(dbg.a - (avgDepths[2]));
	//output.final = (avgNormals[2]/* + avgNormals[1] + avgNormals[2] + avgNormals[3]*/).xyzz;
	//output.final = indexSums / (max(indexSums[0], max(indexSums[1], max(indexSums[2], indexSums[3]))));
	//output.final = visibility.rrrr;
	//output.final = finalLerpValue.rrrr;
	//output.final = avgNormal.xyzz;
	//output.final = avgDepth;
	//output.final = avgColor.xyzz;
	//output.final = avgDepths[3].xxxx;
	//output.final = indirect.xyzz;
	//output.final = indirectFactor.xxxx;
	//output.final = pow(1.0f - directionalFactor, 5.1f).xxxx;

	return output;
}