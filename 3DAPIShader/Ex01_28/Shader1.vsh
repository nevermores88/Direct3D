//  c0-3    = World * View * Projection matrix
//	c4-7	= World matrix
//	c8		= light direction
//	c9		= color
//	c24		= boundary

#define light -c8

vs_1_1

def c24, 1.0, 0.5, 0.1, 0.9

dcl_position v0
dcl_normal v1

m4x4 oPos, v0, c0

m3x3 r0, v1, c4
dp3 r1, r0, light

mad r1.x, r1.x, c24.y, c24.y		//(Dot + 1)*0.5 = Dot * 0.5 + 0.5
max r1.x, r1.x, c24.z
min r1.x, r1.x, c24.w

mov oT0.x, r1.x						// 조명의 계산 결과를 텍스처 좌표 1로 설정

mov oD0, c9