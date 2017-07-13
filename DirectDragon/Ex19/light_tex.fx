float4x4 g_mtWorld;
float4x4 g_mtView;
float4x4 g_mtProj;

texture g_Tex;
sampler DiffuseSampler = sampler_state
{
	Texture = <g_Tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

technique LightAndTexture
{
	pass P0
	{
		pixelshader = null;
		vertexshader = null;

		fvf = XYZ | NORMAL | Tex1;
		Lighting = true;
		NormalizeNormals = true;
		SpecularEnable = false;

		WorldTransform[0] = (g_mtWorld);
		ViewTransform = (g_mtView);
		ProjectionTransform = (g_mtProj);

		LightType[0] = Directional;
		LightAmbient[0] = { 0.2f, 0.2f, 0.2f, 1.0f };
		LightDiffuse[0] = { 1.0f, 1.0f, 1.0f, 1.0f };
		LightSpecular[0] = { 0.0f, 0.0f, 0.0f, 1.0f };
		LightDirection[0] = { 1.0f, -1.0f, 1.0f, 0.0f };
		LightPosition[0] = { 0.0f, 0.0f, 0.0f, 0.0f };
		LightFalloff[0] = 0.0f;
		LightRange[0] = 0.0f;
		LightTheta[0] = 0.0f;
		LightPhi[0] = 0.0f;
		LightAttenuation0[0] = 1.0f;
		LightAttenuation1[0] = 0.0f;
		LightAttenuation2[0] = 0.0f;

		LightEnable[0] = true;

		// Set Material components.  This is like calling
		// IDirect3DDevice9::SetMaterial.
		MaterialAmbient = { 1.0f, 1.0f, 1.0f, 1.0f };
		MaterialDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		MaterialEmissive = { 0.0f, 0.0f, 0.0f, 0.0f };
		MaterialPower = 1.0f;
		MaterialSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };

		// Hook up the sampler object to sampler stage 0,
		// which is given by Sampler[0].

		Sampler[0] = (DiffuseSampler);
	}
}