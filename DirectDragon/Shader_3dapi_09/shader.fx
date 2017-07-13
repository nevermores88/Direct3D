matrix WorldViewProjMatrix;

struct VS_INPUT
{
	vector position : POSITION;
	//vector diffuse : COLOR0;
	float2 tex0 : TEXCOORD0;
};

struct VS_OUTPUT
{
	vector position : POSITION;
	//vector diffuse : COLOR0;
	float2 tex0 : TEXCOORD0;
};

VS_OUTPUT Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(input.position, WorldViewProjMatrix);
	//output.diffuse = input.diffuse;
	output.tex0 = input.tex0;

	return output;
}
