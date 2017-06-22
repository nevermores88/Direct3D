float4x4 g_mtWorld;
float4x4 g_mtView;
float4x4 g_mtProj;
float4x4 g_mtRot;
float3 g_vLightDir;

texture g_ToonTex;
sampler ToonSampler = sampler_state
{
	Texture = <g_ToonTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	//Clamp 적용이 제대로 안되는듯
	AddressU = Clamp;
	AddressV = Clamp;
};

texture g_DiffuseTex;
sampler DiffuseSampler = sampler_state
{
	Texture = <g_DiffuseTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	//Clamp 적용이 제대로 안되는듯
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
	float2 mTexCoord : TEXCOORD0;
	float mToonCoord : TEXCOORD1;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = input.mPosition;
	
	position = mul(position, g_mtWorld);
	position = mul(position, g_mtView);
	position = mul(position, g_mtProj);

	float3 normal = normalize(mul(input.mNormal, g_mtRot));
	float3 lightDir = normalize(-g_vLightDir);

	float diffuse = (0.7f + dot(normal, lightDir)) * 0.5f;

	output.mPosition = position;
	output.mTexCoord = input.mTexCoord;
	output.mToonCoord = saturate(diffuse);

	return output;
}

struct PS_INPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
	float mToonCoord : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 color;

		//color = tex2D( ToonSampler, input.mToonCoord);
		//color *= tex2D( DiffuseSampler, input.mTexCoord);			// Multiply Diffuse Texture

	color = 1.2 * tex2D(ToonSampler, input.mToonCoord);				// Increase Intensity
	color = pow(color, 3) / 1.2f;													// Increase Contrast with using pow()
	color *= tex2D(DiffuseSampler, input.mTexCoord);					// Multiply Diffuse Texture
	color.a = 1;

	output.mColor = color;

	return output;
}

technique Tech0
{
	pass P0
	{
		ALPHABLENDENABLE = FALSE;
		ALPHATESTENABLE = FALSE;
		LIGHTING = FALSE;
		ALPHABLENDENABLE = FALSE;
		CULLMODE = CCW;
		ZWRITEENABLE = TRUE;

		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc();
	}
}