// GLOBALS

cbuffer BufferPerObject
{
	float4x4 gMatWVP;
	float4x4 gMatW;
	float4x4 gMatWInvTransp;
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
	float3 PositionWorld : TEXCOORD1;
	float3 Normal : NORMAL;
	float2 Uv : TEXCOORD0;
};

// MAIN

float4 main(PixelInput input) : SV_Target
{
	return gColBase * float4(abs(input.Normal), 1.0f);
}