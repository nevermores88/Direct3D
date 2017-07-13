matrix WorldViewProj;
sampler smpDif;

struct VS_INPUT
{
	vector position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	vector position: POSITION;
	float2 texCoord : TEXCOORD0;
};

VS_OUTPUT VertexMain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(input.position, WorldViewProj);
	output.texCoord = input.texCoord;

	return output;
}

vector Diffuse = { 1, 1, 1, 1 };

struct PS_INPUT
{
	float2 texCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
	vector diffuse : COLOR0;
};

PS_OUTPUT PixelMain(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.diffuse = tex2D(smpDif, input.texCoord);
	output.diffuse *= Diffuse;

	return output;
}