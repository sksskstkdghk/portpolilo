//셰이더 변수를 특정 레지스터에 수동으로 할당하기위한 선택적 키워드 인덱스 지정하여 구분하여 사용가능
//b	Constant buffer
//t Texture and texturebuffer
//상수버퍼는 구조체와 같은형태를 가진다
cbuffer VS_Vp : register(b0)
{
    matrix View;
    matrix Projection;
}

cbuffer VS_World : register(b1)
{
    matrix World;
}

cbuffer PS_Color : register(b0)
{
    float4 Color;
}

struct VertexInput
{
    float4 Position : POSITION0;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return Color;
}

