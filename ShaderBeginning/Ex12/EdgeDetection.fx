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

VS_OUTPUT EdgeDetection_EdgeDetection_Vertex_Shader_vs_main(VS_INPUT input)
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

float3x3 Kx = { -1, 0, 1,
					-2, 0, 2,
					-1, 0, 1 };

float3x3 Ky = { 1, 2, 1,
					0, 0, 0,
					-1, -2, -1 };

float2 gPixelOffset : ViewportDimensionsInverse;

float4 EdgeDetection_EdgeDetection_Pixel_Shader_ps_main(PS_INPUT input) : COLOR
{
	float Lx = 0;
	float Ly = 0;

	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			float2 offset = float2(x, y)*gPixelOffset;
				float3 tex = tex2D(SceneSampler, input.mUV + offset).rgb;
				float luminance = dot(tex, float3(0.3, 0.59, 0.11));

			Lx += luminance * Kx[y + 1][x + 1];
			Ly += luminance * Ky[y + 1][x + 1];
		}
	}

	float L = sqrt((Lx*Lx) + (Ly*Ly));
	return float4(L.xxx, 1);
}

technique EdgeDetection
{
	pass EdgeDetection
	{
		CULLMODE = NONE;

		VertexShader = compile vs_2_0 EdgeDetection_EdgeDetection_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 EdgeDetection_EdgeDetection_Pixel_Shader_ps_main();
	}

}