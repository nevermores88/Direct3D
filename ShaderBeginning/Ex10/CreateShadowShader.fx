texture ShadowMap_Tex : RenderColorTarget
<
	float2 RenderTargetDimensions = { 2048, 2048 };
	string Format = "D3DFMT_R32F";
	float  ClearDepth = 1.000000;
	int    ClearColor = -1;
>;

struct VS_INPUT
{
	float4 mPosition: POSITION;
};

struct VS_OUTPUT
{
	float4 mPosition: POSITION;
	float4 mClipPosition: TEXCOORD1;
};

float4x4 gWorldMatrix : World;
float4x4 gLightViewMatrix
<
	string UIName = "gLightViewMatrix";
	string UIWidget = "Numeric";
	bool UIVisible = false;
> = float4x4(1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00);
float4x4 gLightProjMatrix : Projection;

float4 gWorldLightPosition;

VS_OUTPUT CreateShadowShader_CreateShadow_Vertex_Shader_vs_main(VS_INPUT input)
{
	VS_OUTPUT output;

	float4x4 lightViewMatrix = gLightViewMatrix;

	output.mPosition = mul(input.mPosition, gWorldMatrix);
	output.mPosition = mul(output.mPosition, lightViewMatrix);
	output.mPosition = mul(output.mPosition, gLightProjMatrix);

	output.mClipPosition = output.mPosition;

	return output;
}

struct PS_INPUT
{
	float4 mClipPosition: TEXCOORD1;
};

float4 CreateShadowShader_CreateShadow_Pixel_Shader_ps_main(PS_INPUT input) : COLOR
{
	float depth = input.mClipPosition.z / input.mClipPosition.w;
	return float4(depth.xxx, 1);
}

technique CreateShadowShader
{
	pass CreateShadow
		<
			string Script = "RenderColorTarget0 = ShadowMap_Tex;"
			"ClearColor = (255, 255, 255, 255);"
			"ClearDepth = 1.000000;";
		>
	{

		VertexShader = compile vs_2_0 CreateShadowShader_CreateShadow_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 CreateShadowShader_CreateShadow_Pixel_Shader_ps_main();
	}

}
