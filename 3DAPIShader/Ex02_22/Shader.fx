sampler DiffuseSampler : register(s0);

struct PS_INPUT
{
	float4 mDiffuse : COLOR0;
	float2 mTex : TEXCOORD0;
};

float4 PxlPrc(PS_INPUT input) : COLOR0
{
	float4 output;

	output = tex2D(DiffuseSampler, input.mTex);
	output *= input.mDiffuse;
	output *= 2;

	return output;
}