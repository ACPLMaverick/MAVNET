/*
*	Deferred map manual:
*	COLOR		(RGB - color) (A - transparency)
*	NORMAL		(XYZ - normal) (W - gloss)
*	WORLDPOS	(XYZ - worldPos)
*	DEPTH		(R - depth)
*/


#define MULTISAMPLE_COUNT 4

struct DVertexInput
{
	float3 Position : POSITION;
	float2 Uv : TEXCOORD0;
};

struct DPixelInput
{
	float4 Position : SV_POSITION;
	float2 Uv : TEXCOORD0;
};

DPixelInput GenerateDPixelInput(uint vertexID)
{
	DPixelInput input;

	/*
	//See: https://web.archive.org/web/20140719063725/http://www.altdev.co/2011/08/08/interesting-vertex-shader-trick/

	1
	( 0, 2)
	[-1, 3]   [ 3, 3]
	.
	|`.
	|  `.
	|    `.
	'------`
	0         2
	( 0, 0)   ( 2, 0)
	[-1,-1]   [ 3,-1]

	ID=0 -> Pos=[-1,-1], Tex=(0,0)
	ID=1 -> Pos=[-1, 3], Tex=(0,2)
	ID=2 -> Pos=[ 3,-1], Tex=(2,0)
	*/

	input.Uv.x = (vertexID == 2) ? 2.0f : 0.0f;
	input.Uv.y = (vertexID == 1) ? 2.0f : 0.0f;

	input.Position = float4(input.Uv * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);

	return input;
}

float3 WorldPositionFromDepth(float4x4 projInverse, float4x4 viewInverse, float2 pixelCoord, float depth)
{
	float4 projectedPos = float4(
		pixelCoord.x * 2.0f - 1.0f, 
		(1.0f - pixelCoord.y) * 2.0f - 1.0f, 
		depth * 2.0f - 1.0f, 
		1.0f);
	float4 viewSpacePos = mul(projInverse, projectedPos);
	viewSpacePos = viewSpacePos / viewSpacePos.w;
	return mul(viewInverse, viewSpacePos).xyz;
	//return float3(-3.9f, 3.9f, -3.9f);
}