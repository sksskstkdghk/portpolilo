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
    matrix boneWorld = mul(bones[index], world);
    
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
    
    if (isNormalMap)
    {
        float4 normalMapping = normalMap.Sample(normalSamp, input.uv);
        
        float3x3 TBN = float3x3(T, B, N);
        
        normal = normalMapping.xyz * 2.0f - 1.0f;
        normal = normalize(mul(normal, TBN));
    }
    
    float diffuseIntensity = saturate(dot(normal, -light));
    
    float specularIntensity = 0;
    if (diffuseIntensity > 0)
    {
        float3 halfWay = normalize(input.viewDir + light);
        specularIntensity = saturate(dot(-halfWay, normal));
        
        specularIntensity = pow(specularIntensity, specExp);
    }
    
    float4 albedo = diffuseMap.Sample(diffuseSamp, input.uv);
    float4 specularMapping = float4(1, 1, 1, 1);
    if (isSpecularMap)
        specularMapping = specularMap.Sample(specularSamp, input.uv);
    
    float4 diffuse = albedo * diffuseIntensity * mDiffuse;
    float4 specular = specularIntensity * mSpecular * specularMapping;
    float4 ambient = ambientLight * albedo * mAmbient;
    
    return diffuse + specular + ambient;
}