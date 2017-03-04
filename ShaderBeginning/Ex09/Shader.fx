float4x4 g_mtWorld : World;
float4x4 g_mtView : View;
float4x4 g_mtProj : Projection;

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

float g_fTime : Time0_X;

float g_fWaveHeight
<
	string UIName = "gWaveHeight";
	string UIWidget = "Numeric";
	bool UIVisible = false;
	float UIMin = -1.00;
	float UIMax = 1.00;
> = float(3.00);

float g_fSpeed
<
	string UIName = "gSpeed";
	string UIWidget = "Numeric";
	bool UIVisible = false;
	float UIMin = -1.00;
	float UIMax = 1.00;
> = float(2.00);

float g_fWaveFrequency
<
	string UIName = "gWaveFrequency";
	string UIWidget = "Numeric";
	bool UIVisible = false;
	float UIMin = -1.00;
	float UIMax = 1.00;
> = float(10.00);

float g_fUVSpeed
<
	string UIName = "gUVSpeed";
	string UIWidget = "Numeric";
	bool UIVisible = false;
	float UIMin = -1.00;
	float UIMax = 1.00;
> = float(0.25);

struct VS_INPUT
{
	float4 m_Pos : POSITION;
	float3 m_Normal : NORMAL;
	float2 m_UV : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 m_Pos : POSITION;
	float2 m_UV : TEXCOORD0;
	float3 m_Diffuse : TEXCOORD1;
	float3 m_ViewDir : TEXCOORD2;
	float3 m_Reflection : TEXCOORD3;
};

VS_OUTPUT UVAnimation_Pass_0_Vertex_Shader_vs_main(VS_INPUT input)
{
	VS_OUTPUT output;

	float cosTime = g_fWaveHeight * cos(g_fTime * g_fSpeed + input.m_UV.x * g_fWaveFrequency);
	input.m_Pos.y += cosTime;

	output.m_Pos = mul(input.m_Pos, g_mtWorld);

	float3 lightDir = output.m_Pos.xyz - g_vLightPos.xyz;
	lightDir = normalize(lightDir);

	float3 viewDir = normalize(output.m_Pos.xyz - g_vCameraPos.xyz);
	output.m_ViewDir = viewDir;

	output.m_Pos = mul(output.m_Pos, g_mtView);
	output.m_Pos = mul(output.m_Pos, g_mtProj);

	float3 worldNormal = mul(input.m_Normal, (float3x3)g_mtWorld);
	worldNormal = normalize(worldNormal);

	output.m_Diffuse = dot(-lightDir, worldNormal);
	output.m_Reflection = reflect(lightDir, worldNormal);

	output.m_UV = input.m_UV + float2(g_fTime * g_fUVSpeed, 0);

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


struct PS_INPUT
{
	float2 m_UV : TEXCOORD0;
	float3 m_Diffuse : TEXCOORD1;
	float3 m_ViewDir: TEXCOORD2;
	float3 m_Reflection : TEXCOORD3;
};

struct PS_OUTPUT
{
	float4 m_Diffuse : COLOR;
};

PS_OUTPUT UVAnimation_Pass_0_Pixel_Shader_ps_main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 albedo = tex2D(DiffuseSampler, input.m_UV);
	float3 diffuse = g_vLightColor * albedo.rgb * saturate(input.m_Diffuse);

	float3 reflection = normalize(input.m_Reflection);
	float3 viewDir = normalize(input.m_ViewDir);
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.0f);

		float4 specularIntensity = tex2D(SpecularSampler, input.m_UV);
			specular *= specularIntensity.rgb * g_vLightColor;
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

	output.m_Diffuse = float4(ambient + diffuse + specular, 1);

	return output;
}

technique UVAnimation
{
	pass Pass_0
	{
		VertexShader = compile vs_2_0 UVAnimation_Pass_0_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 UVAnimation_Pass_0_Pixel_Shader_ps_main();
	}
}