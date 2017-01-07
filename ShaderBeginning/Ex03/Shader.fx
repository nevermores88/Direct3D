//string TextureMapping_Pass_0_Model : ModelData = ".\\Sphere.x";

float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;

struct VS_INPUT
{
	float4 mPos : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 mPos : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT TextureMapping_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.mPos = mul(Input.mPos, gWorldMatrix);
	Output.mPos = mul(Output.mPos, gViewMatrix);
	Output.mPos = mul(Output.mPos, gProjectionMatrix);

	Output.mTexCoord = Input.mTexCoord;

	return Output;
}
texture DiffuseMap_Tex
<
//string ResourceName = "..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Earth.jpg";
string ResourceName = ".\Earth.jpg";
>;

sampler2D DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap_Tex);
};

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
};

float4 TextureMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
	return albedo.rgba;
}

technique TextureMapping
{
	pass Pass_0
	{
		VertexShader = compile vs_2_0 TextureMapping_Pass_0_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 TextureMapping_Pass_0_Pixel_Shader_ps_main();
	}
}
