vs_1_1

//상수 설정
def c10, 1.5, 1.5, 1.5, 1

//정점의 위치, Diffuse 설정
dcl_position	v0		//정점 위치
dcl_normal		v1		//정점 법선
dcl_color		v2		//정점 Diffuse
dcl_texcoord0	v3		//정점 텍스처 좌표0
dcl_texcoord3	v4		//정점 텍스처 좌표1

//입력 레지스터 v0를 임시 레지스터 r0에 복사
mov r0, v0

//c0~c3까지 임시 레지스터에 복사
mov r1, c0
mov r2, c1
mov r3, c2
mov r4, c3

//정점 위치 처리
m4x4 r5, r0, r1		//벡터 * 행렬 연산
mov oPos, r5			//연산 결과를 출력 레지스터에 복사

mov oT7, r5			//픽셀 쉐이더에서 사용할 수 있도록 정점 변환 위치를 oT7에 저장
mov oT6, v1			//픽셀 쉐이더에서 사용할 수 있도록 정점 법선 벡터를 oT6에 저장

mul r0, v2, c10		//r0 = 정점의 색상 * c10
mul r1, r0, c11		//r1 = r0 * c11
mov oD0, r1

mov oT0, v3			//텍스쳐 좌표 0 저장
mov oT1, v4			//텍스쳐 좌표 1 저장