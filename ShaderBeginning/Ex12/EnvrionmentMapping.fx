float4x4 gWorldMatrix : World;
float4x4 gWorldViewProjectionMatrix : WorldViewProjection;

float4 gWorldLightPosition
<
	string UIName = "gWorldLightPosition";
	string UIWidget = "Direction";
	bool UIVisible = false;
	float4 UIMin = float4(-10.00, -10.00, -10.00, -10.00);
	float4 UIMax = float4(10.00, 10.00, 10.00, 10.00);
	bool Normalize = false;
> = float4(500.00, 500.00, -500.00, 1.00);

float4 gWorldCameraPosition : ViewPosition;

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal: NORMAL;
	float3 mTangent: TANGENT;
	float3 mBinormal: BINORMAL;
	float2 mUV: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mUV: TEXCOORD0;
	float3 mLightDir : TEXCOORD1;
	float3 mViewDir: TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

VS_OUTPUT EnvironmentMapping_Pass_0_Vertex_Shader_vs_main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.mPosition = mul(input.mPosition, gWorldViewProjectionMatrix);
	output.mUV = input.mUV;

	float4 worldPosition = mul(input.mPosition, gWorldMatrix);
	float3 lightDir = worldPosition.xyz - gWorldLightPosition.xyz;
	output.mLightDir = normalize(lightDir);

	float3 viewDir = normalize(worldPosition.xyz - gWorldCameraPosition.xyz);
	output.mViewDir = viewDir;

	float3 worldNormal = mul(input.mNormal, (float3x3)gWorldMatrix);
	output.N = normalize(worldNormal);

	float3 worldTangent = mul(input.mTangent, (float3x3)gWorldMatrix);
	output.T = normalize(worldTangent);

	float3 worldBinormal = mul(input.mBinormal, (float3x3)gWorldMatrix);
	output.B = normalize(worldBinormal);

	return output;
}

struct PS_INPUT
{
	float2 mUV : TEXCOORD0;
	float3 mLightDir : TEXCOORD1;
	float3 mViewDir: TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

texture DiffuseMap_Tex;
sampler2D DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
};

texture SpecularMap_Tex;
sampler2D SpecularSampler = sampler_state
{
	Texture = (SpecularMap_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
};

texture NormalMap_Tex;
sampler2D NormalSampler = sampler_state
{
	Texture = (NormalMap_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
};

texture EnvironmentMap_Tex;
samplerCUBE EnvironmentSampler = sampler_state
{
	Texture = (EnvironmentMap_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
};

float3 gLightColor
<
	string UIName = "gLightColor";
	string UIWidget = "Numeric";
	bool UIVisible = false;
	float UIMin = -1.00;
	float UIMax = 1.00;
> = float3(0.70, 0.70, 1.00);

float4 EnvironmentMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT input) : COLOR
{
	float3 tangentNormal = tex2D(NormalSampler, input.mUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);
	tangentNormal = float3(0, 0, 1);

	float3x3 TBN = float3x3(normalize(input.T), normalize(input.B), normalize(input.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

	float4 albedo = tex2D(DiffuseSampler, input.mUV);
	float3 lightDir = normalize(input.mLightDir);
	float3 diffuse = saturate(dot(worldNormal, -lightDir));
	diffuse = gLightColor*albedo.rgb*diffuse;

	float3 viewDir = normalize(input.mViewDir);
	float3 specular = 0;

	if (diffuse.x > 0)
	{
		float3 reflection = reflect(lightDir, worldNormal);

		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.0f);

		float4 specularIntensity = tex2D(SpecularSampler, input.mUV);
		specular *= specularIntensity.rgb * gLightColor;
	}

	float3 viewReflect = reflect(viewDir, worldNormal);
	float3 environment = texCUBE(EnvironmentSampler, viewReflect).rgb;

	float3 ambient = float3(0.1f, 0.1f, 0.1f)*albedo;

	return float4(ambient + diffuse + specular + environment*0.5f, 1);
}

technique EnvironmentMapping
{
	pass Pass_0
	{
		VertexShader = compile vs_2_0 EnvironmentMapping_Pass_0_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 EnvironmentMapping_Pass_0_Pixel_Shader_ps_main();
	}
}