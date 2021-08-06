%include "asm_io.inc"
segment .tex:
	global 	asm_main
asm_main:
	enter	0,0		; setup
	pusha			; setup
	mov	al, 09Ah	; al = 9Ah
	mov	bl, 073h	; bl = 93h
	sub	al, bl		; al = al - bl
	movzx	eax, al		; increase size for printing
	call 	print_int	; print al as an integer
	call	print_nl	; print a new line
	dump_regs	0	; print the registers just for kicks
	popa			; cleanup
	mov	eax, 0		; cleanup
	leave			; cleanup
	ret			; cleanup

