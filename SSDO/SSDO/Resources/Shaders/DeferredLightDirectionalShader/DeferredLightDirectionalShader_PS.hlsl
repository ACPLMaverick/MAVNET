#include "../_global/GlobalDefines.hlsli"
#include "../_global/Lighting.hlsli"

cbuffer LightCommon : register(b0)
{
	float4x4 projInverse;
};

cbuffer LightDirectional : register(b1)
{
	float4 gColor;
	float3 gDirection;
};

Texture2D TexColor : register(t0);
SamplerState SmpColor : register(s0);

Texture2D TexNormal : register(t1);
SamplerState SmpNormal : register(s1);

Texture2D TexDepth : register(t2);
SamplerState SmpDepth : register(s2);

float4 main(DPixelInput input) : SV_TARGET
{
	float4 normalSample = TexNormal.Sample(SmpNormal, input.Uv);

	PixelInput pInput;
	pInput.Position = input.Position;
	pInput.PositionView = ViewPositionFromDepth(projInverse, input.Uv, TexDepth.Sample(SmpDepth, input.Uv).r);
	pInput.Normal = normalize(normalSample.xyz);
	pInput.Uv = input.Uv;

	MaterialData pData;
	pData.colBase = TexColor.Sample(SmpColor, input.Uv);
	pData.colSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	pData.gloss = normalSample.w;

	float4 inColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 viewDir = normalize(-pInput.PositionView);

	LightDirectional(pInput, gColor, gDirection, viewDir, pData, inColor);

	return inColor;
}