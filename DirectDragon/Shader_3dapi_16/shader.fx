struct PS_INPUT
{
	vector diffuse : COLOR;
};

struct PS_OUTPUT
{
	vector diffuse : COLOR;
};

PS_OUTPUT Main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.diffuse = input.diffuse;

	return output;
}