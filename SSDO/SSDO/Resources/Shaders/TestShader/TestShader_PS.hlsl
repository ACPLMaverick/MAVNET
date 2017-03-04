// GLOBALS

cbuffer BufferPerObject
{
	float4 gColBase;
	float4 gColSpecular;
	float gGloss;
};

cbuffer BufferPerFrame
{
	float3 gLightDir;
};

cbuffer BufferInfrequent
{

};

// STRUCTS

struct PixelInput
{
	float4 Position : SV_POSITION;
	float3 PositionWorld : POSITION;
	float3 Normal : NORMAL;
	float2 Uv : TEXCOORD0;
};

// MAIN

float4 main(PixelInput input) : SV_Target
{
	return gColBase * float4(input.Normal, 1.0f);
}