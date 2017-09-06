#include "../_global/GlobalDefines.hlsli"

// Group size lowered to make able for shader to compute two sats at once
#define GROUP_SIZE_X 512
#define GROUP_SIZE_Y 1

// INPUT

cbuffer InputGlobal : register(b1)
{
	const uint gInputLevel;
	const uint gIndexOffset;
	const bool gSampleSecondLayerDifferentialy;
};

Texture2D<float4> InA : register(t4);
Texture2D<float4> InB : register(t5); 
Texture2D<float4> InASat : register(t6);
Texture2D<float4> InBSat : register(t7);

RWTexture2D<float4> OutA : register(u0);
RWTexture2D<float4> OutB : register(u1);
RWTexture2D<float4> OutIndices : register(u2);

// FUNCTIONS

[numthreads(GROUP_SIZE_X, GROUP_SIZE_Y, 1)]
void main( uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex )
{
	
}