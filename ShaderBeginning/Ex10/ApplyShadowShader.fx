struct VS_INPUT
{
	float4 mPosition: POSITION;
	float3 mNormal: NORMAL;
};

struct VS_OUTPUT
{
	float4 mPosition: POSITION;
	float4 mClipPosition: TEXCOORD1;
	float mDiffuse : TEXCOORD2;
};

float4x4 gWorldMatrix : World;
float4x4 gLightViewMatrix
<
	string UIName = "gLightViewMatrix";
	string UIWidget = "Numeric";
	bool UIVisible = false;
> = float4x4(1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00);
float4x4 gLightProjMatrix : Projection;

float4 gWorldLightPosition
<
	string UIName = "gWorldLightPosition";
	string UIWidget = "Direction";
	bool UIVisible = false;
	float4 UIMin = float4(-10.00, -10.00, -10.00, -10.00);
	float4 UIMax = float4(10.00, 10.00, 10.00, 10.00);
	bool Normalize = false;
> = float4(500.00, 500.00, -500.00, 1.00);

float4x4 gViewProjMatrix : ViewProjection;

VS_OUTPUT ApplyShadowShader_ApplyShadowTorus_Vertex_Shader_vs_main(VS_INPUT input)
{
	VS_OUTPUT output;

	float4x4 lightViewMatrix = gLightViewMatrix;

	float4 worldPosition = mul(input.mPosition, gWorldMatrix);
	output.mPosition = mul(worldPosition, gViewProjMatrix);

	output.mClipPosition = mul(worldPosition, lightViewMatrix);
	output.mClipPosition = mul(output.mClipPosition, gLightProjMatrix);

	float3 lightDir = normalize(worldPosition.xyz - gWorldLightPosition.xyz);
		float3 worldNormal = normalize(mul(input.mNormal, (float3x3)gWorldMatrix));
	output.mDiffuse = dot(-lightDir, worldNormal);

	return output;
}

texture ShadowMap_Tex
<
	string ResourceName = ".\\";
>;

sampler2D ShadowSampler = sampler_state
{
	Texture = (ShadowMap_Tex);
};

float4 gObjectColor
<
	string UIName = "gObjectColor";
	string UIWidget = "Color";
	bool UIVisible = true;
> = float4(1.00, 1.00, 0.00, 1.00);

struct PS_INPUT
{
	float4 mClipPosition: TEXCOORD1;
	float mDiffuse : TEXCOORD2;
};

float4 ApplyShadowShader_ApplyShadowTorus_Pixel_Shader_ps_main(PS_INPUT input) : COLOR
{
	float3 rgb = saturate(input.mDiffuse) * gObjectColor;

	float currentDepth = input.mClipPosition.z / input.mClipPosition.w;

	float2 uv = input.mClipPosition.xy / input.mClipPosition.w;
	uv.y = -uv.y;
	uv = uv * 0.5 + 0.5;

	float shadowDepth = tex2D(ShadowSampler, uv).r;

	if (currentDepth > shadowDepth + 0.0000125f)
	{
		rgb *= 0.5f;
	}

	return(float4(rgb, 1.0f));
}

technique ApplyShadowShader
{
	pass ApplyShadowTorus
	{
		VertexShader = compile vs_2_0 ApplyShadowShader_ApplyShadowTorus_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 ApplyShadowShader_ApplyShadowTorus_Pixel_Shader_ps_main();
	}

}


