float4x4 gWorldViewProjection;


struct VS_OUTPUT
{
	float4 mPosition : POSITION;
};

struct VS_INPUT
{
	float4 mPosition :POSITION;
	float3 m_Normal : NORMAL;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	Output.mPosition = mul(Input.mPosition, gWorldViewProjection);

	return Output;
}

struct PS_INPUT
{
	float4 mPosition : POSITION;

};

struct PS_OUTPUT
{
	float4 mDiffuse : COLOR0;
};

PS_OUTPUT ps_main(PS_INPUT Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	Output.mDiffuse = float4(0, 1, 0, 1);
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