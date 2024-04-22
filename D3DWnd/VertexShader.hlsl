struct VS_INPUT
{
	float4 position : POSITION;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
};

int getRandomValue()
{
	return 37;
}

float4 main(VS_INPUT input) : SV_POSITION
{
	int var = getRandomValue();
	return input.position;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}

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
