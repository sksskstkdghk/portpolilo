#include "Header.hlsli"

//원래 쓰던 코드
//cbuffer Color : register(b10)
//{
//    float4 color;
//}

//struct PixelInput
//{
//    float4 position : SV_Position;
//};

//PixelInput VS(Vertex input)
//{
//    PixelInput output;
//    output.position = mul(input.position, world);
//    output.position = mul(output.position, view);
//    output.position = mul(output.position, projection);
    
//    return output;
//}

//float4 PS(PixelInput input) : SV_TARGET
//{
//    return color;
//}

struct InstaceData
{
    Matrix world;
    float4 color;
};

struct VertexInput
{
    float4 position : POSITION;
    
    InstaceData data : INSTANCE;
};

struct PixelInput
{
    float4 position : SV_Position;
    float4 color : COLOR;
    
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    output.position = mul(input.position, input.data.world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.color = input.data.color;
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return input.color;
}