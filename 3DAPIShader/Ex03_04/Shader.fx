float4x4 gWorldViewProj;
float3x3 gWorld;
float3 gLightDir;

texture gDiffuseTex;
sampler DiffuseSampler : register(s0) = 
sampler_state
{
	Texture = (gDiffuseTex);
	AddressU = WRAP;
	AddressV = WRAP;

	MagFilter = LINEAR;
	MinFilter = LINEAR;
	MipFilter = LINEAR;
};

// Vertex Processing
VertexShader VtxPrc = asm
{
	vs_2_0

	def c9, 1.0, 0.5, 0, 1

	dcl_position  v0
	dcl_normal    v1
	dcl_texcoord  v2

	m4x4 oPos, v0, c0	// c0: m_mtWVP

	m3x3 r0.xyz, v1, c4	// c4: m_mtWld. Ratation Transform
	dp3  r1, r0, -c8	// c8: Light Direction. Lambert: r1 = dot(N, -L)

	add  r1, r1, c9.x	//	r1 += 1.0F
	mul  r1, r1, c9.y	//	r1 *= 0.5F	==> r1 = (1 + dot(N,-L))/2
	max  r1, c9.z, r1	//	lower bound 0
	min  r1, c9.w, r1	//	upper bound 1
	mov  oD0, r1

	mov  oT0, v2
};

// Pixel Processing
PixelShader PxlPrc = asm
{
	ps_2_0

	def	   c16, 4, 0, 0, 0

	dcl_2d s0				// declaration 2D sampler s0
	dcl    t0.xy			// declaration 2d texture coordnate
	texld  r0, t0, s0		// sampling

	mul    r0, r0, c16.x	// Modulate
	mov    oC0, r0			// copy to output register 0C0
};

technique Tech0
{
	pass P0
	{
		// Setup Vertex Processing Constant Register
		VertexShaderConstant4[0] = (gWorldViewProj);
		VertexShaderConstant4[4] = (gWorld);
		VertexShaderConstant1[8] = (gLightDir);

		VertexShader = (VtxPrc);



		// Binding Sampler
		Sampler[0] = (DiffuseSampler);

		// Setup Multitexturing Option
		/*
		ColorOp  [0] = MODULATE4X;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;
		AlphaOp  [0] = DISABLE;
		ColorOp  [1] = DISABLE;
		*/

		PixelShader = (PxlPrc);
	}
}