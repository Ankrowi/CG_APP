cbuffer CBuf 
{
	matrix transform;
};

struct VS_IN
{
	float2 pos : POSITION0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	output.pos = mul(float4(input.pos, 0.5f, 1.0f), transform);
	output.col = float4(0.6, 0.2, 0.5, 1.0);;
	return output;
}