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
	const uint3 gWidthHeightLevel;
	const bool gVertical;
};


Texture2D<float4> InA : register(t4);
Texture2D<float4> InB : register(t5);

RWTexture2D<float4> OutA : register(u0);
RWTexture2D<float4> OutB : register(u1);

groupshared float4 TempLocal[GROUP_SIZE_X * 2];

// FUNCTIONS

[numthreads(GROUP_SIZE_X, GROUP_SIZE_Y, 1)]
void main( uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex )
{
	const int texWidth = gWidthHeightLevel[0];
	const int texHeight = gWidthHeightLevel[1];
	const int level = gWidthHeightLevel[2];

	// Cut out unnecessary threads.
	if ((int)DTid.x >= texWidth * 2 || (int)DTid.y >= texHeight * 2)
	{
		return;
	}

	const int iOffset = -1;
	int2 index = DTid.xy;
	int2 indexInFirst = int2(2 * index.x, index.y);
	int2 indexInSecond = int2(2 * index.x + 1, index.y);
	int2 indexOutFirst = int2(2 * index.x + iOffset, index.y);
	int2 indexOutSecond = int2(2 * index.x + 1 + iOffset, index.y);
	if (gVertical)
	{
		index = index.yx;
		indexInFirst = indexInFirst.yx;
		indexOutFirst = indexOutFirst.yx;
		indexInSecond = indexInSecond.yx;
		indexOutSecond = indexOutSecond.yx;
	}


	// Compute a sum of the given row (each thread group corresponds to one row of the texture).
	
	const int thid = DTid.x;
	int offset = 1;

	// Load input into shared memory
	TempLocal[2 * thid] = InA.mips[level][indexInFirst];
	TempLocal[2 * thid + 1] = InA.mips[level][indexInSecond];

	// Build sum in place up the tree (up-sweep)
	for (int d = texWidth >> 1; d > 0; d >>= 1)
	{
		GroupMemoryBarrier();
		if (thid < d)
		{
			int ai = offset * (2 * thid + 1) - 1;
			int bi = offset * (2 * thid + 2) - 1;

			TempLocal[bi] += TempLocal[ai];
		}
		offset *= 2;
	}

	// Clear the last element
	if (thid == 0)
	{
		TempLocal[texWidth - 1] = 0;
	}

	// Traverse down the tree and build scan (down-sweep)
	for (d = 1; d < texWidth; d *= 2)
	{
		offset >>= 1;
		GroupMemoryBarrier();
		if (thid < d)
		{
			int ai = offset * (2 * thid + 1) - 1;
			int bi = offset * (2 * thid + 2) - 1;

			float4 t = TempLocal[ai];
			TempLocal[ai] = TempLocal[bi];
			TempLocal[bi] += t;
		}
	}
	
	GroupMemoryBarrier();




	OutA[indexOutFirst] = TempLocal[2 * thid];
	OutA[indexOutSecond] = TempLocal[2 * thid + 1];

	if (thid == texWidth - 1)
	{
		OutA[index] = InA.mips[level][index] + TempLocal[thid];
	}
}