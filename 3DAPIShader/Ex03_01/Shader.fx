float4x4 gWorldViewProjection;
texture gTexDiffuse;

sampler DiffuseSampler : register(s0) = 
sampler_state
{
	texture = gTexDiffuse;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float4 mDiffuse : COLOR0;
	float2 mTexCoord : TEXCOORD0;
};

struct VS_INPUT
{
	float4 mPosition :POSITION;
	float4 mDiffuse : COLOR0;
	float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	Output.mPosition = mul(Input.mPosition, gWorldViewProjection);
	Output.mDiffuse = Input.mDiffuse;
	Output.mTexCoord = Input.mTexCoord;

	return Output;
}

struct PS_INPUT
{
	//float4 mPosition : POSITION;
	float4 mDiffuse : COLOR0;
	float2 mTexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 mDiffuse : COLOR0;
};

PS_OUTPUT ps_main(PS_INPUT Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	Output.mDiffuse = max(Input.mDiffuse, tex2D(DiffuseSampler, Input.mTexCoord));
	//Output.mDiffuse = tex2D(DiffuseSampler, Input.mTexCoord);
	return Output;
}

technique Tech0
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}