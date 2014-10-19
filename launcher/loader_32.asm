_start:
	jmp string

load:
	mov eax, [esp] ;get return addr = string addr
	xor ecx, ecx   ;caller = NULL
	mov edx, 0x01  ;RTLD_LAZY
	mov ebx, 0x1337;to be replaced with real addr
	call ebx
	add esp, 0x04

string:
	call load
	db "patch.dylib",0x00
