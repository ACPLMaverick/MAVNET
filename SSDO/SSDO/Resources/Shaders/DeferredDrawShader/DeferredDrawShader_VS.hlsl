#include "DeferredDrawShader_H.hlsli"

PixelInput main( VertexInput input )
{
	PixelInput output;
	output.Position = float4(input.Position, 1.0f);
	output.Uv = input.Uv;
	return output;
}