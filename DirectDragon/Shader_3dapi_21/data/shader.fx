sampler SampDif : register(s0);

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
	vector MonoColor = { 0.5f, 1.0f, 2.0f, 1.0f };
	vector MonoWeight = { 0.8f, 0.9f, 1.0f, 0.0f };

	for (int x = -4; x <= 4; ++x)
	{
		float2 T = input.base;
		T.x += (2.f *x) / 1024.f;
		Out.diffuse += tex2D(SampDif, T)*exp((-x*x) / 8.0f);
	}

	Out.diffuse = dot(Out.diffuse, MonoWeight);
	Out.diffuse *= MonoColor;
	Out.diffuse *= 0.18f;
	Out.diffuse.a = 1.0f;

	return Out;
}