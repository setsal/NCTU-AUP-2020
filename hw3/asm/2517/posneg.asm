; ➜ nc aup.zoolab.org 2517
; posneg: test if registers are positive or negative.
;         if ( eax >= 0 ) { var1 = 1 } else { var1 = -1 }
;         if ( ebx >= 0 ) { var2 = 1 } else { var2 = -1 }
;         if ( ecx >= 0 ) { var3 = 1 } else { var3 = -1 }
;         if ( edx >= 0 ) { var4 = 1 } else { var4 = -1 } 
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var3 @ 0x600008-60000c
;       var4 @ 0x60000c-600010
; ======

	cmp eax, 0
	mov eax, 1
	jge l1
	sub eax, 2

l1:
	mov [0x600000], eax
	cmp ebx, 0
	mov eax, 1
	jge l2
	sub eax, 2

l2:
	mov [0x600004], eax
	cmp ecx, 0
	mov eax, 1
	jge l3
	sub eax, 2

l3:
	mov [0x600008], eax
	cmp edx, 0
	mov eax, 1
	jge l4
	sub eax, 2

l4:
	mov [0x60000c], eax
