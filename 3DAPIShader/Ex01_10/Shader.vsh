#define Nor r0
#define Tex r1

vs_1_1

def c32, 0.4, 0.5, 0.0, 1.0

dcl_position v0
dcl_normal v1

m4x4 oPos, v0, c0

m3x3 Nor, v1, c4

mov Tex.xyz, Nor.xyz
mov Tex.w, c32.w
mov Tex.y, -Tex.y

mad Tex, Tex, c32.x, c32.y		//Tex = (1+Tex)/2 텍스쳐 좌표를 (0.5, 0.5) 중심으로 재설정

mov oT0, Tex