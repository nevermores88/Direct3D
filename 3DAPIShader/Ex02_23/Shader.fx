sampler DiffuseSampler1 : register(s0) = sampler_state
{
	MinFilter = POINT;
	MagFliter = POINT;
	MipFilter = POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler DiffuseSampler2 : register(s1) = sampler_state
{
	MinFilter = NONE;
	MagFliter = NONE;
	MipFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler DiffuseSampler3 : register(s2) = sampler_state
{
	MinFilter = LINEAR;
	MagFliter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct PS_INPUT
{
	float4 mDiffuse : COLOR0;
	float2 mTex : TEXCOORD0;
};

float4 PxlPrc(PS_INPUT input) : COLOR0
{
	float4 output;
	float4 t0;
	float4 t1;
	float4 t2;

	t0 = tex2D(DiffuseSampler1, input.mTex);
	t1 = tex2D(DiffuseSampler2, input.mTex);
	t2 = tex2D(DiffuseSampler3, input.mTex);

	output = t0*0.8f + t1*0.6f;
	output *= (t2*1.3f);

	output = pow(output, 1.5) * 1.5;

	output.w = 1;

	return output;
}