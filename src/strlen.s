section .text

global strlen

; NASM Intel Assembly Version

strlen:
	; Check if pointer is null
	test	rdi, rdi
	je		.null_or_empty
	
	; Check if first byte is null terminator
	cmp		byte [rdi], 0
	je		.null_or_empty
	
	; Align pointer to 8-byte boundary
	mov		rax, rdi
	and		rax, 0xfffffffffffffff8
	
	; Load 8 bytes and apply SWAR technique
	mov		rcx, qword [rax]
	mov		rdx, 0x7f7f7f7f7f7f7f7f
	mov		rsi, rcx
	and		rsi, rdx
	add		rsi, rdx
	or		rsi, rcx
	not		rsi
	not		rdx
	and		rdx, rsi
	
	; Shift based on original pointer offset
	lea		ecx, [rdi*8]
	shr		rdx, cl
	
	; Check if null byte found in first 8-byte chunk
	test	rdx, rdx
	je		.main_loop_setup
	
	; Found null byte - calculate position
	bsf		rax, rdx
	shr		rax, 3
	ret

.null_or_empty:
	xor		eax, eax
	ret

.main_loop_setup:
	mov		r8, 0x8080808080808080
	mov		rcx, rax
	sub		rcx, rdi
	add		rax, 8
	mov		rsi, 0xfefefefefefefeff

.main_loop:
	; Load next 8 bytes
	mov		rdi, qword [rax]
	mov		rdx, rdi
	not		rdx
	and		rdx, r8
	add		rdi, rsi
	add		rcx, 8
	add		rax, 8
	and		rdi, rdx
	je		.main_loop
	
	; Found null byte in this chunk
	bsf		rax, rdi
	shr		rax, 3
	add		rax, rcx
	ret
