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

texture g_SpecularTex;
sampler SpecularSampler = sampler_state
{
	Texture = <g_SpecularTex>;
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

	output.mPosition = mul(position, g_mtWorldViewProj);
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

PS_OUTPUT NormalPrc(PS_INPUT input)
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
	//bump *= tex2D(DiffuseSampler, input.mTexCoord);
	bump += 0.4f;

	output.mColor = bump;

	return output;
}

PS_OUTPUT PxlPrc0(PS_INPUT input)
{
	return NormalPrc(input);
}

PS_OUTPUT PxlPrc1(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.mColor = tex2D(SpecularSampler, input.mTexCoord);

	return output;
}

PS_OUTPUT PxlPrc2(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 specularTex = tex2D(SpecularSampler, input.mTexCoord);

	float4 hue = { 1.0, 0.6, 0.3, 1.0 };

	specularTex *= hue;										// Color Shift
	specularTex *= 2.5f;										// Up to Intensity
	specularTex = pow(specularTex, 2.5)*1.5;			// Up to Contrast with using pow() function

	output.mColor = specularTex;

	return output;
}

PS_OUTPUT PxlPrc3(PS_INPUT input, uniform float4 hue)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 diffuse = tex2D(DiffuseSampler, input.mTexCoord);
	float4 bump = NormalPrc(input).mColor;
	float4 specular = tex2D(SpecularSampler, input.mTexCoord);

	specular *= hue;
	specular *= 2.5f;
	specular = pow(specular, 2.5f)* 1.5f;

	diffuse = 4 * diffuse*bump;
	diffuse = pow(diffuse, 2.5f);
	diffuse += specular;

	diffuse *= hue;

	output.mColor = diffuse;

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
		PixelShader = compile ps_3_0 PxlPrc2();
	}

	pass P3
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc3(float4(1.0f, 0.6f, 0.3f, 1.0f));
	}

	pass P4
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc3(float4(0.3f, 1.0f, 0.6f, 1.0f));
	}

	pass P5
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc3(float4(0.6f, 0.3f, 1.0f, 1.0f));
	}
}