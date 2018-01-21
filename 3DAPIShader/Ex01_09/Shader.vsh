//
// Shader Example.
// Copyleft (c) 2007 Heesung Oh(Galic Studio). All rights Not reserved.
////////////////////////////////////////////////////////////////////////////

// c0: World * View * Projection
// c10: Color

vs_1_1

dcl_position 	v0
dcl_color		v2
dcl_texcoord	v3

m4x4 oPos, v0,	c0		// 정점 변환

mul  r0, v2,	c10		// ro = 정점의 색상 * 상수(c10) 곱

mov  oD0, r0			// 출력 디퓨즈 색상
mov oT0, v3				// 출력 UV 좌표 = 입력 UV 좌표

