float4x4 g_mtWorldViewProj;
float3 g_vLightDir;

texture g_DiffuseTex;
sampler DiffuseSampler = sampler_state
{
	Texture = <g_DiffuseTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture g_NormalTex;
sampler NormalSampler = sampler_state
{
	Texture = <g_NormalTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal : NORMAL;
	float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord: TEXCOORD0;
	float3 mN : TEXCOORD1;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = input.mPosition;

	position = mul(position, g_mtWorldViewProj);

	output.mPosition = position;
	output.mTexCoord = input.mTexCoord;
	output.mN = input.mNormal;

	return output;
}

struct PS_INPUT
{
	float2 mTexCoord: TEXCOORD0;
	float3 mN : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR;
};

PS_OUTPUT NormalPrc(PS_INPUT input, uniform int iPower = 1)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 bump = { 1, 1, 1, 1 };
	float3 B = { 1, 0, 0 };
	float3 T = { 0, 1, 0 };
	float3 N = { 0, 0, 1 };

	float3 lightDir = normalize(-g_vLightDir);
	N = normalize(input.mN);
	T = normalize(cross(N, B));
	B = normalize(cross(T, N));

	float3 C1 = 0;
	float3 C = 2 * tex2D(NormalSampler, input.mTexCoord) - 1;
	C = normalize(C);

	C1.x = B.x * C.x + T.x * C.y + N.x * C.z;
	C1.y = B.y * C.x + T.y * C.y + N.y * C.z;
	C1.z = B.z * C.x + T.z * C.y + N.z * C.z;
	C1 = normalize(C1);

	bump = dot(C1, lightDir);
	bump += 0.85f;
	bump = pow(bump, iPower);

	output.mColor = bump;

	return output;
}

PS_OUTPUT PxlPrc0(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	output.mColor = tex2D(DiffuseSampler, input.mTexCoord);

	return output;
}

PS_OUTPUT PxlPrc1(PS_INPUT input)
{
	return NormalPrc(input, 1);
}

PS_OUTPUT PxlPrc2(PS_INPUT input, uniform int iPower)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	output = NormalPrc(input, iPower);
	output.mColor *= tex2D(DiffuseSampler, input.mTexCoord);

	return output;
}

technique Tech0
{
	pass P0
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc0();
	}

	pass P1
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc1();
	}

	pass P2
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc2(1.2);
	}

	pass P3
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc2(2.2);
	}

	pass P4
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc2(4.2);
	}
}