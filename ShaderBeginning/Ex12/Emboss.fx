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

VS_OUTPUT EdgeDetection_Emboss_Vertex_Shader_vs_main(VS_INPUT Input)
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

float3x3 K = { -2, -1, 0,
					-1, 0, 1,
					0, 1, 2 };

float2 gPixelOffset : ViewportDimensionsInverse;

float4 EdgeDetection_Emboss_Pixel_Shader_ps_main(PS_INPUT input) : COLOR
{
	float res = 0;

	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			float2 offset = float2(x, y)*gPixelOffset;
			float3 tex = tex2D(SceneSampler, input.mUV + offset).rgb;
			float luminance = dot(tex, float3(0.3, 0.59, 0.11));

			res += luminance * K[y + 1][x + 1];
		}
	}

	res += 0.5f;

	return float4(res.xxx, 1);
}

technique EdgeDetection
{
	pass Emboss
	{
		CULLMODE = NONE;

		VertexShader = compile vs_2_0 EdgeDetection_Emboss_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 EdgeDetection_Emboss_Pixel_Shader_ps_main();
	}

}