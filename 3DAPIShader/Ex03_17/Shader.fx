float4x4 g_mtWorld;
float4x4 g_mtView;
float4x4 g_mtProj;
float4x4 g_mtRot;
float3 g_vLightDir;
float3 g_vCamPos;
float g_fSharpness;

struct VS_INPUT
{
	float3 mPosition : POSITION;
	float3 mNormal : NORMAL;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float4 mColor : COLOR;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = float4(input.mPosition, 1);
	position = mul(position, g_mtWorld);

	float3 viewDir = normalize(g_vCamPos - position);

		float3 normal = normalize(mul(input.mNormal, g_mtRot));
	float3 lightDir = normalize(-g_vLightDir);
	float3 reflectionDir = normalize(2 * dot(normal, lightDir)*normal - lightDir);
	float4 specular = pow(max(0, dot(reflectionDir, viewDir)), g_fSharpness);

	position = mul(position, g_mtView);
	position = mul(position, g_mtProj);

	specular.a = 1.0f;

	output.mPosition = position;
	output.mColor = specular;

	return output;
}

technique Tech0
{
	pass P0
	{
		VertexShader = compile vs_2_0 VtxPrc();
	}
}