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

VS_OUTPUT ColorConversion_Sepia_Vertex_Shader_vs_main(VS_INPUT Input)
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

float4 ColorConversion_Sepia_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
	float4 tex = tex2D(SceneSampler, Input.mUV);

	float4 sepia;
	sepia.a = tex.a;
	//sepia.r = tex.r * 0.393f + tex.g * 0.769f + tex.b * 0.189f;
	//sepia.g = tex.g * 0.349f + tex.g * 0.686f + tex.b * 0.168f;
	//sepia.b = tex.b * 0.272f + tex.g * 0.534f + tex.b * 0.131f;

	sepia.r = dot(tex.rgb, float3(0.393f, 0.769f, 0.189f));
	sepia.g = dot(tex.rgb, float3(0.349f, 0.686f, 0.168f));
	sepia.b = dot(tex.rbb, float3(0.272f, 0.534f, 0.131f));

	return sepia;
}

technique ColorConversion
{
	pass Sepia
	{
		CULLMODE = NONE;

		VertexShader = compile vs_2_0 ColorConversion_Sepia_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 ColorConversion_Sepia_Pixel_Shader_ps_main();
	}
}