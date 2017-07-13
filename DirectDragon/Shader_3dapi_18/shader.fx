sampler EarthTex;

struct PS_INPUT
{
	float2 base : TEXCOORD0;
};

struct PS_OUTPUT
{
	vector diffuse : COLOR0;
};

PS_OUTPUT Main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.diffuse = tex2D(EarthTex, input.base);

	return output;
}