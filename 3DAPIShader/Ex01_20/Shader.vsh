// c4:	View Matrix
// c8:  Projection Matrix
// c12:	World Matrix Palette

#define MATRIX_OFFSET	12

vs_2_0

def	c0, 4.0, 0.0, 0.0, 1.0

dcl_position v0
dcl_blendweight v1
dcl_blendindices v2
dcl_normal	v3
dcl_color v4
dcl_texcoord v5

mul r0, v2, c0.x		//행렬은 4x4 이므로 상수 레지스터의 위치를 정하기 위해 인덱스에 4를 곱한다.
mova a0, r0

m4x4 r0, v0, c[MATRIX_OFFSET + a0.x]			//행렬 인덱스에 따라 정점을 변환한다.
m4x4 r1, v0, c[MATRIX_OFFSET + a0.y]
m4x4 r2, v0, c[MATRIX_OFFSET + a0.z]
m4x4 r3, v0, c[MATRIX_OFFSET + a0.w]

//비중 계산
//마지막 비중은 1 - (v1.x + v1.y + v1.z)
mov r4, v1
add r4.w, r4.x, r4.y
add r4.w, r4.w, r4.z
add r4.w, c0.w, -r4.w

//변환된 정점의 위치에 각 비중을 곱한다.
mul r0, r0, r4.x
mul r1, r1, r4.y
mul r2, r2, r4.z
mul r3, r3, r4.w

//비중이 곱해진 각 위치를 더한다.
add r0, r0, r1
add r0, r0, r2
add r0, r0, r3

//정점의 위치를 뷰, 투영 변환 적용하고 출력 레지스터에 기록한다.
m4x4 r1, r0, c4			//뷰 변환
m4x4 oPos, r1, c8		//투영 변환

//기타 출력
mov oD0, v4				//Diffuse 출력
mov oT0, v5				//Texture 좌표 출력