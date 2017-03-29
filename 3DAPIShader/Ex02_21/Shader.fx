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

	float4 fDiffuse = 0.0f;
	float4 fMonoColor = { 0.5F, 1.0F, 2.0F, 1.0F };
	float4 fMonoWeight = { 0.8F, 0.9F, 1.0F, 0.0F };

	for (int x = -4; x <= 4; ++x)
	{
		float2 fCoord = input.mTexCoord;
			fCoord.x += (2.0f * x) / 1024.0f;
		fDiffuse += tex2D(DiffuseSampler, fCoord) * exp((-x*x) / 8.0f);
	}

	fDiffuse = dot(fDiffuse, fMonoWeight);
	fDiffuse *= fMonoColor;
	fDiffuse *= 0.18f;
	fDiffuse.a = 1.0f;

	output.mDiffuse = fDiffuse;

	return output;
}