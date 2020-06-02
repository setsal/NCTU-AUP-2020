; bubble: bubble sort for 10 integers
; ======
;       a[0] @ 0x600000-600004
;       a[1] @ 0x600004-600008
;       a[2] @ 0x600008-60000c
;       a[3] @ 0x60000c-600010
;       a[4] @ 0x600010-600014
;       a[5] @ 0x600014-600018
;       a[6] @ 0x600018-60001c
;       a[7] @ 0x60001c-600020
;       a[8] @ 0x600020-600024
;       a[9] @ 0x600024-600028
; ======

mov	esi, 0
mov edi, 9

l1:
	cmp esi, edi
	jg	end
	mov	ecx, 9
	lea eax, [0x600000]

l2:
	lea ebx, [eax+4]
	mov edx, [ebx]
	cmp	[eax], edx
	jg 	swap
	jmp	count

swap:
	xor edx, [eax]
	xor [eax], edx
	xor edx, [eax]
	mov [ebx], edx

count:
	add	eax, 4
	loop l2
	inc	esi
	jmp l1

end: