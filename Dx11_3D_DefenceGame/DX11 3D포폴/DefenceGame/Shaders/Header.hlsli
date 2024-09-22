#define MAX_BONE 256
#define MAX_KEY 500

cbuffer VP : register(b0)
{
    matrix view;
    matrix projection;
    matrix invView;
}

cbuffer W : register(b1)
{
    matrix world;
}

cbuffer Bones : register(b2)
{
    matrix bones[MAX_BONE];
    
    int index;
}

cbuffer BoneIndex : register(b3)
{
    int boneIndex;
}

cbuffer Light : register(b0)
{
    float3 lightDirection;
    float specExp;
    
    float4 ambientLight;
    
    int isSpecularMap;
    int isNormalMap;
}

cbuffer Material : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
}

Texture2D diffuseMap : register(t0);
SamplerState diffuseSamp : register(s0);

Texture2D specularMap : register(t1);
SamplerState specularSamp : register(s1);

Texture2D normalMap : register(t2);
SamplerState normalSamp : register(s2);

struct Vertex
{
    float4 position : POSITION;
};

struct VertexUV
{
    float4 position : POSITION;
    float2 uv : UV;    
};

struct VertexUVColor
{
    float4 position : POSITION;
    float2 uv : UV;
    float4 color : COLOR;
};

struct VertexSize
{
    float4 position : POSITION;
    float2 size : SIZE;
};

struct VertexUVNormal
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct VertexUVNormalTangent
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexUVNormalTangentColor
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 Color : COLOR;
};

struct VertexUVNormalTangentBlend
{
    float4 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 blendIndices : BLENDINDICES;
    float4 blendWeights : BLENDWEIGHTS;
};

float3 CamPos()
{
    return invView._41_42_43;
}

float Random(float seed, float min, float max)
{
    float value = frac(sin(seed) * 10000.0f);
    float random = value * (max - min);
    
    return min + random;
}