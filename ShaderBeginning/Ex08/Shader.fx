float4x4 g_mtWorld : World;
float4x4 g_mtWorldViewProj : WorldViewProjection;
float4 g_vLightPos
<
	string UIName = "gWorldLightPosition";
	string UIWidget = "Direction";
	bool UIVisible = false;
	float4 UIMin = float4(-10.00, -10.00, -10.00, -10.00);
	float4 UIMax = float4(10.00, 10.00, 10.00, 10.00);
	bool Normalize = false;
> = float4(500.00, 500.00, -500.00, 1.00);

float4 g_vCameraPos : ViewPosition;

struct VS_INPUT
{
	float4 m_Pos : POSITION;
	float3 m_Normal: NORMAL;
	float3 m_Tangent: TANGENT;
	float3 m_Binormal: BINORMAL;
	float2 m_UV: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 m_Pos : POSITION;
	float2 m_UV: TEXCOORD0;
	float3 m_LightDir : TEXCOORD1;
	float3 m_ViewDir: TEXCOORD2;
	float3 m_T : TEXCOORD3;
	float3 m_B : TEXCOORD4;
	float3 m_N : TEXCOORD5;
};

VS_OUTPUT EnvironmentMapping_Pass_0_Vertex_Shader_vs_main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)output;

	output.m_Pos = mul(input.m_Pos, g_mtWorldViewProj);
	output.m_UV = input.m_UV;

	float4 worldPosition = mul(input.m_Pos, g_mtWorld);
	float3 lightDir = worldPosition.xyz - g_vLightPos.xyz;
	output.m_LightDir = normalize(lightDir);

	float3 viewDir = normalize(worldPosition.xyz - g_vCameraPos.xyz);
	output.m_ViewDir = viewDir;

	float3 worldNormal = mul(input.m_Normal, (float3x3)g_mtWorld);
	output.m_N = normalize(worldNormal);

	float3 worldTangent = mul(input.m_Tangent, (float3x3)g_mtWorld);
	output.m_T = normalize(worldTangent);

	float3 worldBinormal = mul(input.m_Binormal, (float3x3)g_mtWorld);
	output.m_B = normalize(worldBinormal);

	return output;
}


float3 g_vLightColor
<
string UIName = "gLightColor";
string UIWidget = "Numeric";
bool UIVisible = false;
float UIMin = -1.00;
float UIMax = 1.00;
> = float3(0.70, 0.70, 1.00);

texture g_DiffuseTex;
sampler2D DiffuseSampler = sampler_state
{
	Texture = (g_DiffuseTex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
};

texture g_SpecularTex;
sampler2D SpecularSampler = sampler_state
{
	Texture = (g_SpecularTex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
};

texture g_NormalTex;
sampler2D NormalSampler = sampler_state
{
	Texture = (g_NormalTex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
};

texture g_EnvMapTex;
samplerCUBE EnvironmentSampler = sampler_state
{
	Texture = (g_EnvMapTex);
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct PS_INPUT
{
	float2 m_UV : TEXCOORD0;
	float3 m_LightDir : TEXCOORD1;
	float3 m_ViewDir: TEXCOORD2;
	float3 m_T : TEXCOORD3;
	float3 m_B : TEXCOORD4;
	float3 m_N : TEXCOORD5;
};

struct PS_OUTPUT
{
	float4 m_Diffuse : COLOR;
};

PS_OUTPUT EnvironmentMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float3 tangentNormal = tex2D(NormalSampler, input.m_UV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);
	tangentNormal = float3(0, 0, 1);

	float3x3 TBN = float3x3(normalize(input.m_T), normalize(input.m_B), normalize(input.m_N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

	float4 albedo = tex2D(DiffuseSampler, input.m_UV);
	float3 lightDir = normalize(input.m_LightDir);
	float3 diffuse = saturate(dot(worldNormal, -lightDir));
	diffuse = g_vLightColor * albedo.rgb * diffuse;

	float3 viewDir = normalize(input.m_ViewDir);
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		float3 reflection = reflect(lightDir, worldNormal);

		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.0f);

		float4 specularIntensity = tex2D(SpecularSampler, input.m_UV);
		specular *= specularIntensity.rgb * g_vLightColor;
	}

	float3 viewReflect = reflect(viewDir, worldNormal);
	float3 environment = texCUBE(EnvironmentSampler, viewReflect).rgb;
	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	output.m_Diffuse = float4(ambient + diffuse + specular + environment*0.5f, 1);

	return output;
}

technique EnvironmentMapping
{
	pass Pass_0
	{
		VertexShader = compile vs_2_0 EnvironmentMapping_Pass_0_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 EnvironmentMapping_Pass_0_Pixel_Shader_ps_main();
	}
}