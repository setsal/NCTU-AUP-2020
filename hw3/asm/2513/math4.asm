; ➜ nc aup.zoolab.org 2514
; math5: 32-bit signed arithmetic
;         var3 = (var1 * -var2) / (var3 - ebx)
;         note: overflowed part should be truncated
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var3 @ 0x600008-60000c
; ======


mov eax, [0x600000]
mov ebx, 5
mul ebx
neg eax

mov ecx, eax
mov eax, [0x600004]
neg eax
mov ebx, [0x600008]
cdq
idiv ebx
mov ebx, edx

mov eax, ecx
cdq
idiv ebx
mov [0x60000c], eax

done: