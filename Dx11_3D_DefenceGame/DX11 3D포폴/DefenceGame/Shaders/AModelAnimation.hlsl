#include "Header.hlsli"

struct KeyFrame
{
    int clip;
    
    uint curFrame;
    uint nextFrame;
    
    float time;
    float runningTime;
    
    float speed;
    
    float2 padding;
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
    TweenFrame tweenFrame;
}

Texture2DArray boneTransforms : register(t10);

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexUVNormalTangentBlend input)
{
    PixelInput output;
    
    matrix transform = 0;
    matrix cur = 0, curAnim = 0;
    matrix next = 0, nextAnim = 0;
    
    float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
    float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };
    
    int clip[2];
    uint curFrame[2];
    uint nextFrame[2];
    float time[2];
    
    clip[0] = tweenFrame.cur.clip;
    curFrame[0] = tweenFrame.cur.curFrame;
    nextFrame[0] = tweenFrame.cur.nextFrame;
    time[0] = tweenFrame.cur.time;
    
    clip[1] = tweenFrame.next.clip;
    curFrame[1] = tweenFrame.next.curFrame;
    nextFrame[1] = tweenFrame.next.nextFrame;
    time[1] = tweenFrame.next.time;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        c0 = boneTransforms.Load(int4(indices[i] * 4 + 0, curFrame[0], clip[0], 0));
        c1 = boneTransforms.Load(int4(indices[i] * 4 + 1, curFrame[0], clip[0], 0));
        c2 = boneTransforms.Load(int4(indices[i] * 4 + 2, curFrame[0], clip[0], 0));
        c3 = boneTransforms.Load(int4(indices[i] * 4 + 3, curFrame[0], clip[0], 0));
        cur = matrix(c0, c1, c2, c3);
        
        n0 = boneTransforms.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0));
        n1 = boneTransforms.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0));
        n2 = boneTransforms.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0));
        n3 = boneTransforms.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0));
        next = matrix(n0, n1, n2, n3);
        
        curAnim = lerp(cur, next, time[0]);
        
        [flatten]
        if(clip[1] > -1)
        {
            c0 = boneTransforms.Load(int4(indices[i] * 4 + 0, curFrame[1], clip[1], 0));
            c1 = boneTransforms.Load(int4(indices[i] * 4 + 1, curFrame[1], clip[1], 0));
            c2 = boneTransforms.Load(int4(indices[i] * 4 + 2, curFrame[1], clip[1], 0));
            c3 = boneTransforms.Load(int4(indices[i] * 4 + 3, curFrame[1], clip[1], 0));
            cur = matrix(c0, c1, c2, c3);
        
            n0 = boneTransforms.Load(int4(indices[i] * 4 + 0, nextFrame[1], clip[1], 0));
            n1 = boneTransforms.Load(int4(indices[i] * 4 + 1, nextFrame[1], clip[1], 0));
            n2 = boneTransforms.Load(int4(indices[i] * 4 + 2, nextFrame[1], clip[1], 0));
            n3 = boneTransforms.Load(int4(indices[i] * 4 + 3, nextFrame[1], clip[1], 0));
            next = matrix(n0, n1, n2, n3);
        
            nextAnim = lerp(cur, next, time[1]);
            
            curAnim = lerp(curAnim, nextAnim, tweenFrame.tweenTime);
        }
        
        transform += mul(weights[i], curAnim);
    }
    
    matrix boneWorld = mul(transform, world);
    
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