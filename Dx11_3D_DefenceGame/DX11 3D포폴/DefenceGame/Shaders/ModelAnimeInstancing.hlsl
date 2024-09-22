#include "Header.hlsli"

struct KeyFrame
{
    int clip;
    
    uint curFrame;
    uint nextFrame;
    
    float time;
    float runningTime;
    
    float speed;
    
    float2 Padding;
};

struct TweenFrame
{
    float takeTime;
    float tweenTime;
    float runningTime;
    float padding;
    
    KeyFrame cur;
    KeyFrame next;
};

cbuffer Frame : register(b10)
{
    TweenFrame tweenFrame[500];
}

Texture2DArray transformMap : register(t10);

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 blendIndices : BLENDINDICES;
    float4 blendWeights : BLENDWEIGHTS;
    
    matrix transform : INSTANCE;
    uint instanceID : SV_InstanceID;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
    
    uint instanceID : UINT;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    matrix transform = 0;
    matrix cur = 0, curAnim = 0;
    matrix next = 0, nextAnim = 0;
    
    float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
    float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };
    
    [flatten]
    if (any(input.blendIndices) == false)
    {
        indices[0] = index;
        weights[0] = 1.0f;
    }
    
    int clip[2];
    uint curFrame[2];
    uint nextFrame[2];
    float time[2];
    
    clip[0] = tweenFrame[input.instanceID].cur.clip;
    curFrame[0] = tweenFrame[input.instanceID].cur.curFrame;
    nextFrame[0] = tweenFrame[input.instanceID].cur.nextFrame;
    time[0] = tweenFrame[input.instanceID].cur.time;
    
    clip[1] = tweenFrame[input.instanceID].next.clip;
    curFrame[1] = tweenFrame[input.instanceID].next.curFrame;
    nextFrame[1] = tweenFrame[input.instanceID].next.nextFrame;
    time[1] = tweenFrame[input.instanceID].next.time;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[0], clip[0], 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[0], clip[0], 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[0], clip[0], 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[0], clip[0], 0));
        cur = matrix(c0, c1, c2, c3);
        
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0));
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0));
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0));
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0));
        next = matrix(n0, n1, n2, n3);
        
        curAnim = lerp(cur, next, saturate(time[0]));
        
        [flatten]
        if (clip[1] > -1)
        {
            c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[1], clip[1], 0));
            c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[1], clip[1], 0));
            c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[1], clip[1], 0));
            c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[1], clip[1], 0));
            cur = matrix(c0, c1, c2, c3);
        
            n0 = transformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], clip[1], 0));
            n1 = transformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], clip[1], 0));
            n2 = transformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], clip[1], 0));
            n3 = transformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], clip[1], 0));
            next = matrix(n0, n1, n2, n3);
        
            nextAnim = lerp(cur, next, time[1]);
            
            curAnim = lerp(curAnim, nextAnim, tweenFrame[input.instanceID].tweenTime);
        }
        
        transform += mul(weights[i], curAnim);
    }
    
    matrix boneWorld = mul(transform, input.transform);
    
    output.position = mul(input.position, boneWorld);
    
    float3 camPos = invView._41_42_43;
    output.viewDir = normalize(output.position.xyz - camPos);
    
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) boneWorld);
    output.tangent = mul(input.tangent, (float3x3) boneWorld);
    output.binormal = cross(output.normal, output.tangent);
    
    output.instanceID = input.instanceID;
    
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