int m_nMulti;

sampler sampler0 : register(s0) = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler sampler1 : register(s1) = sampler_state
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

	vector t0 = tex2D(sampler0, input.base);
	vector t1 = tex2D(sampler1, input.base);

	if (0 == m_nMulti)
		Out.diffuse = t0;
	else if (1 == m_nMulti)
		Out.diffuse = t1;
	else if (2 == m_nMulti)
		Out.diffuse = t0*t1;
	else if (3 == m_nMulti)
		Out.diffuse = t0*t1 * 2;
	else if (4 == m_nMulti)
		Out.diffuse = t0*t1 * 4;
	else if (5 == m_nMulti)
		Out.diffuse = t0 + t1;
	else if (6 == m_nMulti)
		Out.diffuse = t0 + t1 - 0.5f;
	else if (7 == m_nMulti)
		Out.diffuse = (t0 + t1 - 0.5f) * 2;
	else if (8 == m_nMulti)
		Out.diffuse = t0 + t1 - t0*t1;
	else if (9 == m_nMulti)
		Out.diffuse = t0 - t1;
	else if (10 == m_nMulti)
		Out.diffuse = t1 - t0;
	else if (11 == m_nMulti)
		Out.diffuse = 1 - t0;
	else if (12 == m_nMulti)
		Out.diffuse = 1 - t1;
	else if (13 == m_nMulti)
		Out.diffuse = 1 - (t0 + t1);

	return Out;
}