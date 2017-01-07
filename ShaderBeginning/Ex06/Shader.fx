float4x4 g_mtWorldViewProj : WorldViewProjection;
float4x4 g_mtWorldInv : WorldInverse;

float4 g_vLightPos
<
string UIName = "g_vLightPos";
string UIWidget = "Direction";
bool UIVisible = false;
float4 UIMin = float4(-10.00, -10.00, -10.00, -10.00);
float4 UIMax = float4(10.00, 10.00, 10.00, 10.00);
bool Normalize = false;
> = float4(500.00, 500.00, -500.00, 1.00);


float3 g_vSurfaceColor
<
string UIName = "g_vSurfaceColor";
string UIWidget = "Numeric";
bool UIVisible = false;
float UIMin = -1.00;
float UIMax = 1.00;
> = float3(0.00, 1.00, 0.00);

struct VS_INPUT
{
	float4 m_Pos : POSITION;
	float3 m_Normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 m_Pos : POSITION;
	float3 m_Diffuse : TEXCOORD1;
};

VS_OUTPUT ToonShader_Pass_0_Vertex_Shader_vs_main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.m_Pos = mul(input.m_Pos, g_mtWorldViewProj);

	float3 vLocalLightPos = mul(g_vLightPos, g_mtWorldInv);
	float3 vLightDir = normalize(input.m_Pos.xyz - vLocalLightPos);

	output.m_Diffuse = dot(-vLightDir, normalize(input.m_Normal));
	
	return(output);
}

struct PS_INPUT
{
	float3 m_Diffuse : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 m_Diffuse : COLOR;
};

PS_OUTPUT ToonShader_Pass_0_Pixel_Shader_ps_main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float3 diffuse = saturate(input.m_Diffuse);

	diffuse = ceil(diffuse * 5) / 5.0f;
	output.m_Diffuse = float4(g_vSurfaceColor * diffuse.xyz, 1);
	return output;

}

technique ToonShader
{
	pass Pass_0
	{
		VertexShader = compile vs_2_0 ToonShader_Pass_0_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 ToonShader_Pass_0_Pixel_Shader_ps_main();
	}

}