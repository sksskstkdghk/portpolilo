#include "Header.hlsli"

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexUVNormalTangent input)
{
    PixelInput output;
    matrix boneWorld = bones[boneIndex];
    
    output.position = mul(input.position, boneWorld);
    
    float3 camPos = invView._41_42_43;
    output.viewDir = normalize(output.position.xyz - camPos);
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) boneWorld);
    output.tangent = mul(input.tangent, (float3x3) boneWorld);
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float3 light = normalize(lightDirection);
    
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
    
    float3 normal = N;
    
    float diffuseIntensity = saturate(dot(normal, -light));
    
    float4 albedo = diffuseMap.Sample(diffuseSamp, input.uv);
    
    float4 diffuse = albedo * diffuseIntensity * mDiffuse;
    float4 ambient = ambientLight * albedo * mAmbient;
    
    return diffuse + ambient;
}