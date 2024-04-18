struct VS_INPUT
{
	float4 position : POSITION;
	float4 color : COLOR0;
};

struct PS_INPUT
{
	float4 position : SV_Position;
}

float4 main(VS_INPUT input) : SV_POSITION
{
	return input.position;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}