vs_1_1

dcl_position v0

m4x4 r0, v0, c0

rcp r1.w, r0.w
mul r0, r0, r1.w
mov oPos, r0

m4x4 r1, r0, c8			//�ؽ��� ��ǥ [0, 1]�� ��ȯ

mov oT0, r1