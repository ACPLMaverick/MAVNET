float4x4 MatWorldViewProj : register (c0);

// IO Structs

struct VS_INPUT
{
    float4 Position : POSITION;
    float2 UV0 : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float2 UV0  : TEXCOORD0;  // vertex texture coords 
};

///////////////////////////////
// functions

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT Output;
    
    Output.Position = mul(input.Position, MatWorldViewProj);
    Output.UV0 = input.UV0;
    
    return Output;    
}