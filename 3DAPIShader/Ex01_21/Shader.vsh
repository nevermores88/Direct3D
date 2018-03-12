// c0: world * view * projection
// c4: world * view
// c10: Diffuse
// c12: Fog

vs_1_1

#define FogBegin c12.x
#define FogEnd c12.y
#define FogDeltaReciprocal c12.w   // 1/(FogEnd - FogBegin)

def c14, 1.0, 0.0, 0.0, 0.0

dcl_position v0
dcl_color v2
dcl_texcoord v3

m4x4 oPos, v0, c0
mov oT0, v3
mul oD0, v2, c10

m4x4 r0, v0, c4

//Fog Factor = distance/(end - begin)
sub r0.z, FogEnd, r0.z						//(fog end - distance)
mul r0.x, r0.z, FogDeltaReciprocal		//distance/(end - begin)

min r0.x, r0.x, c14.x				//1보다 큰 값 제거
max r0.x, r0.x, c14.y			//음수 값 제거

mov oFog, r0.x					//포그 출력 레지스터에 저장

