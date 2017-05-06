int m_nMulti;

sampler SampDif0 : register(s0) = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler SampDif1 : register(s1) = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

float4 PxlPrc(float4 Tx0 : TEXCOORD0) : COLOR0
{
	float4 output = 0;
	float4 t0 = tex2D(SampDif0, Tx0);			// Sampling m_TxDif0
	float4 t1 = tex2D(SampDif1, Tx0);			// Sampling m_TxDif1

	if (0 == m_nMulti)		output = t0;
	else if (1 == m_nMulti)	output = t1;
	else if (2 == m_nMulti)	output = t0 * t1;			// Modulate
	else if (3 == m_nMulti)	output = t0 * t1 * 2;		// Modulate 2x
	else if (4 == m_nMulti)	output = t0 * t1 * 4;		// Modulate 4x
	else if (5 == m_nMulti)	output = t0 + t1;			// Add
	else if (6 == m_nMulti)	output = t0 + t1 - .5;		// Add signed
	else if (7 == m_nMulti)	output = (t0 + t1 - .5) * 2;	// Add signed
	else if (8 == m_nMulti)	output = t0 + t1 - t0*t1;	// add smooth
	else if (9 == m_nMulti)	output = t0 - t1;			// sub
	else if (10 == m_nMulti)	output = t1 - t0;			// sub
	else if (11 == m_nMulti)	output = 1 - t0;			// Inverse t0
	else if (12 == m_nMulti)	output = 1 - t1;			// Inverse t1
	else if (13 == m_nMulti)	output = 1 - (t0 + t1);	// Inverse (t0+t1)

	return output;
}