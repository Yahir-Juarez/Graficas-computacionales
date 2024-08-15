Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

sampler animationData : register(s1);

float4 animationDataParameter;

float4x4 selectorConstant;

cbuffer WVP : register(b0)
{
    float4x4 View;
    float4x4 Proj;
}

struct VS_INPUT
{
	float3 position : POSITION;
    //float3 Normal : NORMAL0;
	float2 tex : TEXCOORD0;
    //float4 Color : COLOR0;
	
    float4 blendWeight : BLENDWEIGHT;
    float4 blendIndices : BLENDINDICES;
    
    //----------------------------------------------------//
    
    float4 instance_Quaternion : TEXCOORD1;
    float4 instance_PositionAndScale : TEXCOORD2;
    float2 instance_ScaleYZ : TEXCOORD3;
    float4 instance_ColorAndId : COLOR0;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
    //float3 texW : TEXCOORD0;
    //float3 Normal : TEXCOORD1;
	float2 tex : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input) 
{
	PS_INPUT output = (PS_INPUT) 0;
	
    output.position = mul(float4(input.position.xyz, 1.0f), mul(View, Proj));
	
	//output.position = input.position;
	output.tex = input.tex;
	return output;
}

//VS_INPUT MeshVS (VS_INPUT v)
//{
//    float3 finalVertexPos = float3(0, 0, 0);
	
//    for (int i = 0; i < 4; i++)
//    {
//        float blendIndice = dot(v.blendIndices, selectorConstant[i]);
//        float blendWeight = dot(v.blendWeight, selectorConstant[i]);
        
//        float4 boneT = (animationDataParameter.xy, +animationDataParameter.zw * float2(blendIndice * 2 + 0, instanceID)).xyyy;

//    }

//}

float4 mainPS(PS_INPUT input) : SV_Target
{
	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
	return txDiffuse.Sample(samLinear, input.tex);
}




//struct VS_INPUT
//{
//	float4 position : POSITION;
//};
//
//struct PS_INPUT
//{
//	float4 position : SV_POSITION;
//};
//
//int getRandomValue()
//{
//	return 37;
//}
//
//float4 main(VS_INPUT input) : SV_POSITION
//{
//	int var = getRandomValue();
//	return input.position;
//}
//
//float4 mainPS(PS_INPUT input) : SV_Target
//{
//	return float4(1.0f, 1.0f, 0.0f, 1.0f);
//}





//// Pixel Shader
//float4 PS(PS_INPUT input) : SV_Target
//{
//    return txDiffuse.Sample(samLinear, input.Tex) * vMeshColor;
//}
//
//// Vertex Shader
//PS_INPUT VS(VS_INPUT input)
//{
//    PS_INPUT output = (PS_INPUT)0;
//    output.Pos = mul(input.Pos, World);
//    output.Pos = mul(output.Pos, View);
//    output.Pos = mul(output.Pos, Projection);
//    output.Tex = input.Tex;
//
//    return output;
//}

