; ➜ nc aup.zoolab.org 2515
; minicall: implement a minimal function call in the emulator
; ===== THE CODE
;     call   a
;     jmp    exit
; 
; a:  ; function a - read ret-addr in rax
;     pop    rax
;     push   rax
;     ret
; exit:
; ======

    call   l1
    jmp    exit

l1:
    pop    rax
    push   rax
    ret

exit: