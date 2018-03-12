// c0: world * view * projection
// c4: world * view
// c12: Fog Factor

vs_1_1

#define FogBegin c12.x
#define FogEnd c12.y
#define FogDeltaReciprocal c12.w   // 1/(FogEnd - FogBegin)

def c14, 1.0, 0.0, 0.0, 0.0

dcl_position v0
dcl_color v2
dcl_texcoord v3

m4x4 r0, v0, c0
mov oPos, r0
mov oD0, v2

mul r0.x, v0.y, FogDeltaReciprocal		//Output FogFactor : height / (end - begin)

min r0.x, r0.x, c14.x
max r0.x, r0.x, c14.y

mov oFog, r0.x

