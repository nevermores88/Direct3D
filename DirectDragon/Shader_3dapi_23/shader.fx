sampler sample0 : register(s0) = sampler_state
{
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler sample1 : register(s1) = sampler_state
{
	MinFilter = NONE;
	MagFilter = NONE;
	MipFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler sample2 : register(s2) = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

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
	PS_OUTPUT Out = (PS_OUTPUT)0;

	vector t0;
	vector t1;
	vector t2;

	t0 = tex2D(sample0, input.base);
	t1 = tex2D(sample1, input.base);
	t2 = tex2D(sample2, input.base);

	Out.diffuse = t0*0.8f + t1 * 0.6f;
	Out.diffuse *= (t2*1.3f);

	Out.diffuse = pow(Out.diffuse, 1.5f) * 1.5f;

	Out.diffuse.w = 1;

	return Out;
}