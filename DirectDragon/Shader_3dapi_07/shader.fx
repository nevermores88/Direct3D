matrix WorldViewProjMatrix;

struct VS_INPUT
{
	vector position : POSITION;
	vector diffuse : COLOR;
};

struct VS_OUTPUT
{
	vector position : POSITION;
	vector diffuse : COLOR;
};

VS_OUTPUT Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(input.position, WorldViewProjMatrix);

	output.diffuse = input.diffuse;

	return output;
}