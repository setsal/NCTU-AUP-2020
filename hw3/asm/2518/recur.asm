; ➜ nc aup.zoolab.org 2518   
; recur: implement a recursive function
; 
;    r(n) = 0, if n <= 0
;         = 1, if n == 1
;         = 2*r(n-1) + 3*r(n-2), otherwise
;    
;    please call r(24) and store the result in RAX
; ======
	
	push 24
	call l1
	jmp not

l1:
	mov rax, [rsp + 8]
	cmp rax, 0
	jle fin
	cmp rax, 1
	je return
	
	sub rax, 1
	push rax
	call l1
	mov rcx, 2
	mul rcx
	mov [rsp], rax				
						
	mov rax, [rsp + 0x10]
	sub rax, 2
	push rax			
	call l1
	add rsp, 8
	mov rcx, 3
	mul rcx
	
	pop rdx
	add rax, rdx

return:
	ret

fin:
	mov rax, 0
	jmp return

not:
	nop