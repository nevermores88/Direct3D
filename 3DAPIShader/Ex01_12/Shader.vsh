#define Nor r0
#define Lgt -c8

vs_1_1

def c24, 1.0, 0.5, 0.1, 1.0

dcl_position v0
dcl_normal v1

m4x4 oPos, v0, c0
m3x3 Nor, v1, c4

dp3 r1.w, Nor, Lgt	//(-)Light 방향과 내적으로 정점의 밝기를 설정

//add r1, r1, c24.x	//(Dot + 1)*0.5로 설정해서 0~1.0 사이의 값으로 조정
//mul r1, r1, c24.y
mad r1, r1.w, c24.y, c24.y

//max r1, r1, c24.z	//밝기를 [0.1, 1] 범위로 조정
//min	r1, r1, c24.w

mov r1.w, c24.w
mul oD0, r1, c10		//최종 색상 = Lambert 반사 * 빛의 색상

