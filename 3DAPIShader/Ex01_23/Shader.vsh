// c0: world * view * projection
// c4: world * view
// c12: Fog Factor
// c13: Fog Color

vs_1_1

#define FogBegin c12.x
#define FogEnd c12.y
#define FogFct c12.z							//FogEnd/(FogEnd - FogBegin)
#define FogDeltaReciprocal c12.w			//1/(FogEnd - FogBegin)
#define FogColor c13

def c14, 1.0, 0.0, 0.0, 0.0

dcl_position v0
dcl_color v2

m4x4 oPos, v0, c0								//Output Position

mad r0.x, -v0.y, FogDeltaReciprocal, FogFct			// FogFactor = -Height * 1/(FogEnd - Begin) + FogEnd / (FogEnd - Begin)

min r0.x, r0.x, c14.x
max r0.x, r0.x, c14.y

//FogColor * FogFactor + Diffuse *(1-FogFactor)
mul r1, FogColor, r0.x

add r2, c14.x, -r0.x

mad oD0, v2, r2, r1