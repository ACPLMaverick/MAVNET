// GLOBALS

cbuffer BufferPS
{
	float4 gColBase;
	float4 gColSpecular;
	float gGloss;
};

// STRUCTS

struct PixelInput
{
	float4 Position : SV_POSITION;
	float3 PositionWorld : TEXCOORD1;
	float3 Normal : NORMAL;
	float2 Uv : TEXCOORD0;
};

struct PixelOutput
{
	float4 Color    : SV_Target0;
	float4 Normal	: SV_Target1;
	float4 WorldPos : SV_Target2;
};


// MAIN

PixelOutput main(PixelInput input)
{
	PixelOutput output;
	output.Color = gColBase;
	output.Normal = float4(normalize(input.Normal), gGloss);
	output.WorldPos = float4(input.PositionWorld, 1.0f);
	return output;
}