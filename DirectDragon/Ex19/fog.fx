technique Fog
{
	pass P0
	{
		pixelshader = null;
		vertexshader = null;
		fvf = XYZ | Normal;
		Lighting = true;
		NormalizeNormals = true;
		SpecularEnable = false;

		FogVertexMode = LINEAR;
		FogStart = 50.0f;
		FogEnd = 300.0f;
		FogColor = 0x00CCCCCC;
		FogEnable = true;
	}
}