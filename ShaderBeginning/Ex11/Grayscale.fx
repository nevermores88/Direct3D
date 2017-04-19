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

VS_OUTPUT ColorConversion_Grayscale_Vertex_Shader_vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.mPosition = Input.mPosition;
	Output.mUV = Input.mUV;

	return Output;
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

float4 ColorConversion_Grayscale_Pixel_Shader_ps_main(PS_INPUT input) : COLOR
{
	float4 tex = tex2D(SceneSampler, input.mUV);

	//tex.rgb = (tex.r + tex.g + tex.b ) / 3;
	//tex.rgb = tex.r * 0.3 + tex.g * 0.59 + tex.b * 0.11;
	tex.rgb = dot(tex.rgb, float3(0.3, 0.59, 0.11));

	return tex;
}

technique ColorConversion
{
	pass Grayscale
	{
		CULLMODE = NONE;

		VertexShader = compile vs_2_0 ColorConversion_Grayscale_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 ColorConversion_Grayscale_Pixel_Shader_ps_main();
	}
}