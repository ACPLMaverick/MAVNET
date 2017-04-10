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
