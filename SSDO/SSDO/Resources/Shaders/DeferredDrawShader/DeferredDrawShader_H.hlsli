struct VertexInput
{
	float3 Position : POSITION;
	float2 Uv : TEXCOORD0;
};

struct PixelInput
{
	float4 Position : SV_POSITION;
	float2 Uv : TEXCOORD0;
};
