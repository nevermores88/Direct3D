float4x4 gWorldViewProj;
float3x3 gWorld;
float3 gLightDir;

//Vertex Processing
VertexShader VtxPrc = asm
{
	vs_2_0 //vs_3_0 으로 하면 에러남

	def c9, 1.0, 0.5, 0, 1

	dcl_position  v0
	dcl_normal    v1
	dcl_texcoord  v2

	m4x4 oPos, v0, c0

	m3x3 r0.xyz, v1, c4  //m3x3 r0, v1, c4 하면 에러남 r0 뒤에 .xyz 붙여줘야함
	dp3  r1, r0, -c8	// Lambert: r1 = dot(N, -L)

	add  r1, r1, c9.x	//	r1 += 1.0F
	mul  r1, r1, c9.y	//	r1 *= 0.5F	==> r1 = (1 + dot(N,-L))/2
	max  r1, c9.z, r1	//	lower bound 0
	min  r1, c9.w, r1	//	upper bound 1
	mov  oD0, r1

	mov  oT0, v2
};

technique Tech0
{
	pass P0
	{
		VertexShaderConstant4[0] = (gWorldViewProj);
		VertexShaderConstant4[4] = (gWorld);
		VertexShaderConstant1[8] = (gLightDir);

		VertexShader = <VtxPrc>;
	}
};