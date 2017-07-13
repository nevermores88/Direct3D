extern float4x4 g_mtWorldView;
extern float4x4 g_mtWorldViewProj;

extern float4 g_Color;
extern float3 g_vLightDir;

extern texture g_Tex;
sampler DiffuseSampler = sampler_state
{
	Texture = (g_Tex);
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;
};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal : NORMAL;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
	float4 mColor : COLOR;
};

VS_OUTPUT Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.mPosition = mul(input.mPosition, g_mtWorldViewProj);

	float4 vLightDir = mul(float4(g_vLightDir, 0), g_mtWorldView);
	float4 normal = mul(float4(input.mNormal, 0), g_mtWorldView);

	float u = dot(normalize(vLightDir), normalize(normal));

	if (u < 0.0f)
		u = 0.0f;

	float v = 0.5f;

	output.mTexCoord.x = u;
	output.mTexCoord.y = v;

	output.mColor = g_Color;

	return output;
}

technique Toon
{
	pass P0
	{
		VertexShader = compile vs_2_0 Main();

		Sampler[0] = (DiffuseSampler);
	}
}