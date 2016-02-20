
// Global variables
float4 AmbientColor;
float4 DiffuseColor;     

texture DiffuseMap;
                 
float4x4 MatWorldViewProj;

// Texture samplers
sampler DiffuseMapSampler = 
sampler_state
{
    Texture = <DiffuseMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

/////////////////////////////////
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

struct PS_OUTPUT
{
    float4 Color : COLOR0;  // Pixel color    
};

///////////////////////////////
// functions

VS_OUTPUT MainVS(VS_INPUT input)
{
    VS_OUTPUT Output;
    
    Output.Position = mul(input.Position, MatWorldViewProj);
    Output.UV0 = input.UV0;
    
    return Output;    
}

PS_OUTPUT MainPS(VS_OUTPUT input) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    Output.Color = saturate(tex2D(DiffuseMapSampler, input.UV0) * DiffuseColor + AmbientColor);
    //Output.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return Output;
}

//////////////////////////////////////////

technique Main
{
    pass Pass0
    {          
        VertexShader = compile vs_1_1 MainVS();
        PixelShader  = compile ps_2_0 MainPS(); 
    }
}