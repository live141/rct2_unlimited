_start:
	jmp string

load:
	mov rax, [rsp] ;get return addr = string addr
	xor rcx, rcx   ;caller = NULL
	mov rdx, 0x01  ;RTLD_LAZY
	mov rbx, 0x1337;to be replaced with real addr
	call rbx
	add rsp, 0x04

string:
	call load
	db "patch.dylib",0x00
