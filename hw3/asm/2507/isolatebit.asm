➜ nc aup.zoolab.org 2507
; isolatebit:
;         get the value bit-11 ~ bit-5 in AX and store the result in val1
;         (zero-based bit index)
; ======
;       val1 @ 0x600000-600001
;       val2 @ 0x600001-600002
; ======

mov cx, ax
mov ebx, [0x600001]
and cx, 0xFE0
shr cx, 5 
mov [0x600000], cx
mov [0x600001], ebx