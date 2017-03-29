sampler2D DiffuseSampler;

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 mDiffuse : COLOR0;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	output.mDiffuse = tex2D(DiffuseSampler, input.mTexCoord);
	return output;
}