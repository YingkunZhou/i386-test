; This program computes prime numbers
%include 'asm_io.inc'

segment .data
	Message	db     "Find primes up to: ", 0

segment .bss
	Limit	resd	1      ; 4-byte int
	Guess	resd	1      ; 4-byte int

segment .text
	global	asm_main
asm_main:
	enter	0, 0		; setup
	pusha			; setup

	mov	eax, Message	; print the message
	call	print_string
	call	read_int	; read Limit
	mov	[Limit], eax
	mov	eax, 2		; print “2\n”
	call	print_int
	call 	print_nl
	mov	eax, 3		; print “3\n”
	call	print_int
	call	print_nl
	mov	dword  [Guess], 5	; Guess = 5

while_limit:
	mov	eax, [Guess]
	cmp	eax, [Limit]	; compare Guess and Limit
	jnbe	end_while_limit	; If !(Guess <= Limit) Goto end_while_limit

	mov	ebx, 3		; ebx is factor
while_factor:
	mov	eax, ebx		; eax = factor
	mul	eax			; edx:eax = factor * factor
	cmp	edx, 0			; compare edx and 0
	jne	end_while_factor	; factor too big
	cmp	eax, [Guess]		; compare  factor*factor and guess
	je	endif			; if == then the nuber is a perfect square
	jnb	end_while_factor	; if !< then the number is prime
	mov	edx, 0			; edx = 0
	mov	eax, [Guess] 		; eax = [Guess]
	div	ebx			; divide edx:eax by factor
	cmp	edx, 0			; compare the remainder with 0
	je	endif	                ; if == 0 goto endif
	add	ebx, 2			; factor += 2
	jmp	while_factor		; loop back
end_while_factor:
	mov	eax, [Guess]		; print guess
	call	print_int		; print guess
	call	print_nl		; print guess
endif:
	add	dword [Guess], 2	; guess += 2

	jmp	while_limit		; Loop again

end_while_limit:
	popa			; clean up
	mov	eax, 0		; clean up
	leave			; clean up
	ret			; clean up
