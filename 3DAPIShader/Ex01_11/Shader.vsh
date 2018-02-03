vs_1_1

dcl_position v0

m4x4 r0, v0, c0

rcp r1.w, r0.w
mul r0, r0, r1.w
mov oPos, r0

m4x4 r1, r0, c8			//텍스쳐 좌표 [0, 1]로 변환

mov oT0, r1