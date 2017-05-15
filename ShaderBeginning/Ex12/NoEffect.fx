struct VS_INPUT
{
	float4 mPosition: POSITION;
	float2 mUV : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 mPosition: POSITION;
	float2 mUV : TEXCOORD0;
};

VS_OUTPUT ColorConversion_NoEffect_Vertex_Shader_vs_main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.mPosition = input.mPosition;
	output.mUV = input.mUV;

	return output;
}

struct PS_INPUT
{
	float2 mUV : TEXCOORD0;
};

texture SceneTexture_Tex;
sampler2D SceneSampler = sampler_state
{
	Texture = (SceneTexture_Tex);
};

float4 ColorConversion_NoEffect_Pixel_Shader_ps_main(PS_INPUT input) : COLOR
{
	float4 tex = tex2D(SceneSampler, input.mUV);

	return tex;
}

technique ColorConversion
{
	pass NoEffect
	{
		CULLMODE = NONE;

		VertexShader = compile vs_2_0 ColorConversion_NoEffect_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 ColorConversion_NoEffect_Pixel_Shader_ps_main();
	}
}