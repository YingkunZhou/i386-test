; This simple program takes 10 integers from the user and
; counts how many of these integers are odd

%include "asm_io.inc"

segment .data
	msg_odd	db	"The number of odd numbers is: ",0

segment .bss
	integers resd	10	; space to store 10 32-bit integers

segment .text
	global	asm_main
asm_main:
	enter	   0,0		; set up
	pusha			; set up

    	push	integers	; we pass integers to get_integers
	push	dword 10	; we pass the number of integers to get_integers 
	call	get_integers	; call get_integers
	add	esp, 8		; clean up the stack (often done as op ecx twice, which may be quicker)
	mov	eax, msg_odd	; store the address of the message to print into eax
	call	print_string	; print the message
	push 	integers	; we pass integers to count_odds	
	push	10		; we pass the number of integers to count_odds
	call	count_odds	; call count_odds
	add	esp, 8		; clean up the stack
	call	print_int	; print the content of eax as an integer (this is what count_odds returned)
	call	print_nl	; print a new line

	popa			; clean up
	mov	   eax, 0	; clean up
	leave			; clean up
	ret			; clean up

;   	FUNCTION: Get_Integers
; 	Takes two parameters:  an address in memory in which to store integers, and a number of integers to store (>0)
; 	Destroys values of eax, ebx, and ecx!!

segment .data
	msg_int		db	"Enter an integer: ",0

segment .text
get_integers:
	push	ebp		; save the value of EBP of the caller
	mov	ebp, esp	; update the value of EBP for this subprogram

	mov	ecx, [ebp + 12]	; ECX = address at which to store the integers (parameter #1)
	mov	ebx, [ebp + 8]	; EBX = number of integers to read (parameter #2)
	shl	ebx, 2		; EBX = EBX * 4  (unsigned)
	add	ebx, ecx	; EBX = ECX + EBX = address beyond that of the last integer to be stored
loop1:
	mov	eax, msg_int	; EAX = address of the message to print
	call	print_string;	; print the message
	call	read_int	; read an integer from the keyboard (which will be stored in EAX)
	mov	[ecx], eax	; store the integer in memory at the correct address
	add	ecx, 4		; ECX = ECX + 4 
	cmp	ecx, ebx 	; compare ECX, EBX
	jb	loop1		; if ECX < EBX, jump to loop1   (unsigned)

	pop	ebp		; restore the value of EBP
	ret			; clean up

;	FUNCTION: count_odds
; 	Takes two parameters:  an address in memory in which integers are stored, and the number of integers (>0)
; 	Destroys values of eax, ebx, and edx!!  (eax = returned value)

segment .text
count_odds:
	push	ebp		; save the value of EBP of the caller
	mov	ebp, esp	; update the value of EBP for this subprogram

	mov	eax, [ebp + 12]	; ECX = address at which integers are stored (parameter #1)
	mov	ebx, [ebp + 8]	; EBX = number of integers (parameter #2)
	shl	ebx, 2		; EBX = EBX * 4  (unsigned)
	add	ebx, eax	; EBX = ECX + EBX = address beyond that of the last integer
	sub	ebx, 4		; EBX = EBX - 4 = address of the last integer
	xor	edx, edx	; EDX = 0
loop2:

	push	dword [ebx] 	; store the current integer on the stack
	call	is_odd		; call is_odd  
	add	esp, 4		; clean up the stack
	add	edx, eax	; EDX += EAX  (EAX = 0 if even, EAX = 1 if odd)
	sub	ebx, 4		; ECX = ECX - 4 
	cmp	ebx, [ebp+12] 	; compare EBX and the address of the first integer
	jnb	loop2		; if EBX >= [EBP+12]  jump to loop2   (unsigned)
	
	mov	eax, edx	; EAX = EDX (= number of odd integers)
	
	pop	ebp		; restore the value of EBP
	ret			; clean up


;  	FUNCTION: is_odd
; 	Takes one parameter:  an integers (>0)
; 	Destroys values of eax and ecx (eax = returned value)

segment .text
is_odd:
	push	ebp		; save the value of EBP of the caller
	mov	ebp, esp	; update the value of EBP for this subprogram

	mov	eax, 0		; EAX = 0
	mov	ecx, [ebp+8]	; EBX = integer (parameter #1)
	shr	ecx, 1		; Right logical shift
	adc	eax, 0		; EAX = EAX + carry  (if even: EAX = 0, if odd: EAX = 1)
	
	pop	ebp		; restore the value of EBP
	ret			; clean up

