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

Texture2D AverageNormalDepth : register(t4);
SamplerState SmpAverageNormalDepth : register(s4);

Texture2D AverageColor : register(t5);
SamplerState SmpAverageColor : register(s5);

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

void GetAverageValues(in const float2 uv, out float3 avgColor, out float3 avgNormal, out float3 avgViewPos, out float avgDepth)
{
	float4 avgNormalDepth = AverageNormalDepth.Sample(SmpAverageNormalDepth, uv);
	float4 avgColor4 = AverageColor.Sample(SmpAverageColor, uv);
	
	avgColor = avgColor4.rgb;
	avgNormal = normalize(avgNormalDepth.xyz);
	avgDepth = avgNormalDepth.w;
	avgViewPos = ViewPositionFromDepth(gProjInverse, uv, avgDepth);
}

void ApplyOcclusionFaloff(in const float depth, in const float diffZ, inout float occlusion)
{
	occlusion = max(occlusion, 0.0f);
	occlusion *= 1.0f - smoothstep(1.0f, 1.0f + 0.3f, occlusion);
	//occlusion *= 1.0f - step(0.02f * pow(1.0f - depth, 0.8f), diffZ);
	occlusion *= step(0.0001f, diffZ);
}

void GetOcclusion(in const float3 avgNormal, in const float3 avgViewPos, in const float3 pxViewPos,
	in const float2 uv, in const float pixelDepth, in const float avgDepth, out float occlusion)
{
	const float3 dirToAvg = avgViewPos - pxViewPos;
	const float diffZ = max(avgDepth - pixelDepth, 0.0f);

	// Non normal-sensitive method.
	float zb = pixelDepth;
	float zt = pixelDepth * (1.0f + gSampleBoxHalfSize);
	occlusion = (zb - avgDepth) / (zb - zt);

	//occlusion = max(dot(dirToAvg, avgNormal), 0.0f);

	occlusion /= gSampleBoxHalfSize;
	occlusion = pow(occlusion, gPowFactor);
	ApplyOcclusionFaloff(pixelDepth, diffZ, occlusion);

	// Fixing bleeding on surfaces with normals close to (0, 1, 0).
	occlusion *= smoothstep(0.1f, 0.2f, (1.0f - dot(avgNormal, float3(0.0f, 1.0f, 0.0f))));
}

void GetBleedingFix(in const float3 viewPos, in const float2 uv, in const float depth, in const float avgDepth, out float fix)
{
	const float fixSize = gSampleBoxHalfSize * depth * 0.0f;
	const uint samplePointCount = 4;
	float fixBleed = 0.0f, fixEdge = 0.0f;

	//float2 samplePoints[samplePointCount] =
	//{
	//	float2(uv.x, uv.y - fixSize),
	//	float2(uv.x + fixSize, uv.y),
	//	float2(uv.x, uv.y + fixSize),
	//	float2(uv.x - fixSize, uv.y)
	//};

	//float4 samples[samplePointCount] =
	//{
	//	AverageNormalDepth.Sample(SmpAverageNormalDepth, samplePoints[0]),
	//	AverageNormalDepth.Sample(SmpAverageNormalDepth, samplePoints[1]),
	//	AverageNormalDepth.Sample(SmpAverageNormalDepth, samplePoints[2]),
	//	AverageNormalDepth.Sample(SmpAverageNormalDepth, samplePoints[3])
	//};

	//const float sampleOutOfBoundsThres = 0.01f;
	//float outOfBoundsFixes[samplePointCount] =
	//{
	//	(step(sampleOutOfBoundsThres, samplePoints[0].y)),
	//	(1.0f - step(1.0f - sampleOutOfBoundsThres, samplePoints[1].x)),
	//	(1.0f - step(1.0f - sampleOutOfBoundsThres, samplePoints[2].y)),
	//	(step(sampleOutOfBoundsThres, samplePoints[3].x))
	//};

	//float diffs[samplePointCount] =
	//{
	//	abs(depth - samples[0].w) * outOfBoundsFixes[0],
	//	abs(depth - samples[1].w) * outOfBoundsFixes[1],
	//	abs(depth - samples[2].w) * outOfBoundsFixes[2],
	//	abs(depth - samples[3].w) * outOfBoundsFixes[3]
	//};

	//const float fixThresholdA = 0.1f * depth;
	//const float fixThresholdB = 0.2f * depth;

	//// Bleed fix.

	//fixBleed = smoothstep(fixThresholdA, fixThresholdB, diffs[0]) +
	//	smoothstep(fixThresholdA, fixThresholdB, diffs[1]) +
	//	smoothstep(fixThresholdA, fixThresholdB, diffs[2]) +
	//	smoothstep(fixThresholdA, fixThresholdB, diffs[3]);
	//fixBleed = 1.0f - saturate(fixBleed);

	float4 avgNormalDepthLowerMip = AverageNormalDepth.SampleLevel(SmpAverageNormalDepth, uv, 1);
	float diffZ = max(avgNormalDepthLowerMip.w - depth, 0.0f);
	float thres = pow(depth, 0.8f) * 0.05f;
	fixEdge = smoothstep(thres - 0.001f, thres, diffZ);
	//fixEdge *= (1.0f - fixBleed);

	fix = 1.0f - fixEdge;
}

void GetIndirecity(in const float3 color, in const float3 avgColor, in const float directionalFactor, in const float occlusion, out float3 indirect)
{
	// Indirecity.
	// Basically a difference between pixel color and average color is calculated.
	// Then it is multiplied by indirect factor, calculated totally not similar to SSDO, 
	// but with average normal and average position used.
	indirect = avgColor;
	indirect = color.xyz - avgColor;
	indirect = saturate(indirect);

	// FAST VERSION
	//indirect = (1.0f - indirect) * saturate(length(indirect)) * avgColor * gLightColor.xyz;
	// ACCURATE VERSION
	indirect = RGBtoHSV(indirect);
	indirect.r = 0.785398f - indirect.r;
	indirect = HSVtoRGB(indirect);
	indirect *= avgColor * gLightColor.xyz * pow(1.0f - directionalFactor, 3.0f) * (1.0f - 2.0f * occlusion) * 0.5f;
}

PixelOutput main(in DPixelInput input)
{
	const float colorSampleBoxMultiplier = 1.5f;

	const float4 color = TexColor.SampleLevel(SmpColor, input.Uv, 0);
	const float4 normalSample = TexNormalDepth.SampleLevel(SmpNormalDepth, input.Uv, 0);
	const float depth = normalSample.w;
	const float3 normal = normalSample.xyz;
	const float3 viewPos = ViewPositionFromDepth(gProjInverse, input.Uv, depth);


	float3 avgColor, avgNormal, avgViewPos;
	float avgDepth = 1.0f;
	GetAverageValues(input.Uv, avgColor, avgNormal, avgViewPos, avgDepth);

	float occlusion;
	GetOcclusion(avgNormal, avgViewPos, viewPos, input.Uv, depth, avgDepth, occlusion);

	float bleedFix;
	GetBleedingFix(viewPos, input.Uv, depth, avgDepth, bleedFix);

	occlusion *= bleedFix;

	MaterialData pData;
	pData.colBase = float4(1.0f, 1.0f, 1.0f, 1.0f);
	pData.colSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	pData.gloss = 50.0f;

	float4 smpColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	const float visibility = 1.0f - occlusion;
	float directionalFactor = saturate(pow((1.0f - max(dot(avgNormal, gLightDirection), 0.0f)), 5.5f));
	float finalLerpValue = occlusion * directionalFactor * gOcclusionPower;
	smpColor = visibility * float4(normalize(gLightColor.xyz), 1.0f);


	float3 indirect;
	GetIndirecity(color.rgb, avgColor, directionalFactor, occlusion, indirect);

	PixelOutput output;
	const float4 colorInput = TexInput.Sample(SmpInput, input.Uv);
	smpColor *= colorInput;

	output.final = lerp(colorInput, smpColor, finalLerpValue) + float4(indirect.xyz, 1.0f);
	//output.final = avgNormal.xyzz;
	//output.final = visibility.xxxx * color;
	//output.final = bleedFix.xxxx;

	return output;
}