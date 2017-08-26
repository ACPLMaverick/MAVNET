#include "../_global/GlobalDefines.hlsli"

#define GROUP_SIZE_X 1024
#define GROUP_SIZE_Y 1

// STRUCTS

struct TempBufferData
{
	float4 NormalDepth;
	float4 Color;
};

// INPUT

cbuffer InputGlobal : register(b1)
{
	uint3 gWidthHeightLevel;
	bool gVertical;
};


Texture2D<float4> InA : register(t4);
Texture2D<float4> InB : register(t5);

RWTexture2D<float4> OutA : register(u0);
RWTexture2D<float4> OutB : register(u1);

globallycoherent RWStructuredBuffer<TempBufferData> TempGlobal : register(u2);

groupshared float4 TempLocal[GROUP_SIZE_X * 2];

// FUNCTIONS

[numthreads(GROUP_SIZE_X, GROUP_SIZE_Y, 1)]
void main( uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex )
{
	const uint texWidth = gWidthHeightLevel[0];
	const uint texHeight = gWidthHeightLevel[1];
	const uint level = gWidthHeightLevel[2];

	// Cut out unnecessary threads.
	if (DTid.x >= texWidth * 2 || DTid.y >= texHeight * 2)
	{
		return;
	}

	// Sample input values.
	float4 inputValueA = InA.mips[level][DTid.xy];
	float4 inputValueB = InB.mips[level][DTid.xy];

	// Save output
	OutA[DTid.xy] = inputValueA;
	OutB[DTid.xy] = inputValueB;
}