vs_1_1

dcl_position	v0					//정점 위치를 입력 레지스터 v0에 선언
dcl_color		v2					//정점 색상을 입력 레지스터 v2에 선언
dcl_texcoord	v3					//정점 UV 좌표를 입력 레지스터 v3에 선언

m4x4 oPos, v0, c0				//정점의 변환 : 출력 위치 = 입력 위치 * c0에 입력된 행렬

mul r0, v2, c10					//r0 = 정점의 색상 * 상수(c10)의 곱

mov oD0, r0						//출력 디퓨즈 색상
mov oT0, v3						//출력 UV 좌표 = 입력 UV 좌표