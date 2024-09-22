#include "Header.hlsli"

Texture2D map : register(t10);
SamplerState samp : register(s10);

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : UV;
	float4 color : COLOR;
};

PixelInput VS(VertexUVColor input)
{
    PixelInput output;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    output.uv = input.uv;
	output.color = input.color;
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 data = map.Sample(samp, input.uv);
    
	if (data.a < 0.5f)
		return input.color;
    
    return map.Sample(samp, input.uv);
}