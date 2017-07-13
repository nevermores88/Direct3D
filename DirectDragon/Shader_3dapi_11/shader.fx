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

	output.position = input.position;
	output.diffuse = input.diffuse;

	return output;
}