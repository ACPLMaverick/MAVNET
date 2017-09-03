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
	occlusion *= smoothstep(-(1.0f + gOcclusionFalloff), -1.0f, -occlusion);
	occlusion *= step(0.0001f, diffZ);
}

// Will probably be used in the future when adaptive layering is implemented.
void SampleDepth(in float2 uv, out float depth)
{
	depth = 1.0f - SatNormalDepth.Sample(SmpSatNormalDepth, uv).w;
}


void GetAverageValues(in const float2 uv, in const float depth, in const float colorSampleMultiplier,
	out float3 avgColor, out float3 avgNormal, out float3 avgViewPos, out float avgDepth)
{
	const uint sampleCount = 4;
	float boxHalfSize = gSampleBoxHalfSize * pow(depth, 0.8f);
	const float2 uvSamples[sampleCount] =
	{
		saturate(float2(uv.x - boxHalfSize, uv.y - boxHalfSize)),
		saturate(float2(uv.x + boxHalfSize, uv.y - boxHalfSize)),
		saturate(float2(uv.x + boxHalfSize, uv.y + boxHalfSize)),
		saturate(float2(uv.x - boxHalfSize, uv.y + boxHalfSize))
	};	// TL, TR, BR, BL

	boxHalfSize *= colorSampleMultiplier;
	const float2 uvSamplesColor[sampleCount] =
	{
		saturate(float2(uv.x - boxHalfSize, uv.y - boxHalfSize)),
		saturate(float2(uv.x + boxHalfSize, uv.y - boxHalfSize)),
		saturate(float2(uv.x + boxHalfSize, uv.y + boxHalfSize)),
		saturate(float2(uv.x - boxHalfSize, uv.y + boxHalfSize))
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
		colors[i] = SatColor.Sample(SmpSatColor, uvSamplesColor[i]).rgb;
	}

	// SAT: (BR - BL - TR + TL) / area
	const float areaRec = 1.0f / ( (uvSamples[2].x - uvSamples[3].x) * gSatDimensions.x * (uvSamples[2].y - uvSamples[1].y) * gSatDimensions.y );
	const float areaRecColor = areaRec / (colorSampleMultiplier * colorSampleMultiplier);

	const float4 avgNormalDepth = (normalsDepths[2] - normalsDepths[3] - normalsDepths[1] + normalsDepths[0]) * areaRec;
	avgNormal = normalize(avgNormalDepth.xyz);
	avgDepth = avgNormalDepth.w;
	avgViewPos = ViewPositionFromDepth(gProjInverse, uv, avgDepth);

	avgColor = (colors[2] - colors[3] - colors[1] + colors[0]) * areaRecColor;
}

float GetOcclusion(in const float3 avgNormal, in const float3 avgViewPos, in const float3 pxViewPos,
	in const float2 uv, in const float pixelDepth, in const float avgDepth)
{
	const float diffZ = abs(pixelDepth - avgDepth);

	const float3 dirToAvg = avgViewPos - pxViewPos;

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

	float3 avgColor, avgNormal, avgViewPos;
	float avgDepth;
	GetAverageValues(input.Uv, depth, colorSampleBoxMultiplier, avgColor, avgNormal, avgViewPos, avgDepth);

	float occlusion = GetOcclusion(avgNormal, avgViewPos, viewPos, input.Uv, depth, avgDepth);

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

	//output.final = visibility.rrrr;
	//output.final = finalLerpValue.rrrr;
	//output.final = avgNormal.xyzz;
	output.final = lerp(colorInput, smpColor, finalLerpValue) + float4(indirect.xyz, 1.0f);
	//output.final = smpColor;
	//output.final = avgColor.xyzz;
	//output.final = indirect.xyzz;
	//output.final = indirectFactor.xxxx;
	//output.final = pow(1.0f - directionalFactor, 5.1f).xxxx;

	return output;
}