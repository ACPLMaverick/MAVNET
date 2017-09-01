#include "../_global/GlobalDefines.hlsli"

#define GROUP_SIZE_X 1024
#define GROUP_SIZE_Y 1

#define NUM_BANKS 64
#define LOG_NUM_BANKS 6
#define CONFLICT_FREE_OFFSET(n) \
	((n) >> NUM_BANKS + (n) >> (2 * LOG_NUM_BANKS))

//#define CONFLICT_FREE_OFFSET(n) 0

// STRUCTS

struct TempBufferData
{
	float4 NormalDepth;
	float4 Color;
};

// INPUT

cbuffer InputGlobal : register(b1)
{
	const uint3 gWidthWidthpowLevel;
	const bool gVertical;
};


Texture2D<float4> InA : register(t4);

RWTexture2D<float4> OutA : register(u0);

groupshared float4 TempLocal[GROUP_SIZE_X * 2];

// FUNCTIONS

[numthreads(GROUP_SIZE_X, GROUP_SIZE_Y, 1)]
void main( uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex )
{
	const uint texWidthPow = gWidthWidthpowLevel[1];
	const uint texWidthPowDivTwo = texWidthPow / 2;
	const uint texWidth = gWidthWidthpowLevel[0];
	const uint level = gWidthWidthpowLevel[2];


	
	const uint iOffset = -1;
	uint2 index = DTid.xy;
	uint2 indexInFirst = uint2(index.x, index.y);
	uint2 indexInSecond = uint2(index.x + texWidthPowDivTwo, index.y);
	uint2 indexOutFirst = uint2(index.x + iOffset, index.y);
	uint2 indexOutSecond = uint2(index.x + texWidthPowDivTwo + iOffset, index.y);
	
	if (gVertical)
	{
		index = index.yx;
		indexInFirst = indexInFirst.yx;
		indexOutFirst = indexOutFirst.yx;
		indexInSecond = indexInSecond.yx;
		indexOutSecond = indexOutSecond.yx;
	}


	// Compute a sum of the given row (each thread group corresponds to one row of the texture).
	
	const uint thid = DTid.x;
	uint offset = 1;

	// Load input into shared memory
	uint localIndexFirst = thid;
	uint localIndexSecond = thid + texWidthPowDivTwo;
	localIndexFirst += CONFLICT_FREE_OFFSET(localIndexFirst);
	localIndexSecond += CONFLICT_FREE_OFFSET(localIndexSecond);
	TempLocal[localIndexFirst] = InA.mips[level][indexInFirst];
	TempLocal[localIndexSecond] = InA.mips[level][indexInSecond];

	// Build sum in place up the tree (up-sweep)
	for (uint d = texWidthPow >> 1; d > 0; d >>= 1)
	{
		GroupMemoryBarrierWithGroupSync();
		if (thid < d)
		{
			uint ai = offset * (2 * thid + 1) - 1;
			uint bi = offset * (2 * thid + 2) - 1;
			ai += CONFLICT_FREE_OFFSET(ai);
			bi += CONFLICT_FREE_OFFSET(bi);

			TempLocal[bi] += TempLocal[ai];
		}
		offset *= 2;
	}

	// Clear the last element
	if (thid == 0)
	{
		TempLocal[texWidthPow - 1 + CONFLICT_FREE_OFFSET(texWidthPow - 1)] = 0;
	}

	// Traverse down the tree and build scan (down-sweep)
	for (d = 1; d < texWidthPow; d *= 2)
	{
		offset >>= 1;
		GroupMemoryBarrierWithGroupSync();
		if (thid < d)
		{
			uint ai = offset * (2 * thid + 1) - 1;
			uint bi = offset * (2 * thid + 2) - 1;
			ai += CONFLICT_FREE_OFFSET(ai);
			bi += CONFLICT_FREE_OFFSET(bi);

			float4 t = TempLocal[ai];
			TempLocal[ai] = TempLocal[bi];
			TempLocal[bi] += t;
		}
	}
	
	GroupMemoryBarrierWithGroupSync();

	OutA[indexOutFirst] = TempLocal[localIndexFirst];
	OutA[indexOutSecond] = TempLocal[localIndexSecond];

	if (thid == texWidthPow - 1)
	{
		OutA[index] = InA.mips[level][index] + TempLocal[localIndexFirst];
	}
}