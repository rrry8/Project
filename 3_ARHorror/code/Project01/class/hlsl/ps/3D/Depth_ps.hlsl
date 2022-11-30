struct PS_INPUT
{
    float4 svpos : SV_POSITION;
    float3 pos   : POSITION;
    float3 norm  : NORMAL;
    float3 tan   : TANGENT;
    float3 bin   : BINORMAL;
    float2 uv    : TECOORD;
    float3 col   : COLOR;
};

struct PS_OUTPUT
{
    float4 color : SV_TARGET;
};

cbuffer CBuffer : register(b13)
{
    float4 cfDOF_Area;
}

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return output;
}