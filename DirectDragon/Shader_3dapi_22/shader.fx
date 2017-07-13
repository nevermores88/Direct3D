sampler sample1 : register(s0);

struct PS_INPUT
{
	vector diffuse : COLOR0;
	float2 texCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
	vector diffuse : COLOR0;
};

PS_OUTPUT Main(PS_INPUT input)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;

	Out.diffuse = tex2D(sample1, input.texCoord);
	//Out.diffuse *= input.diffuse;
	Out.diffuse *= 2;

	return Out;
}