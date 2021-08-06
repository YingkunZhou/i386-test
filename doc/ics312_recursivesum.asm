; This simple program takes 10 integers from the user and
; counts how many of these integers are odd

%include "asm_io.inc"

segment .data
	msg1		db	"Enter n: ", 0
	msg2		db	"The sum is: ", 0

segment .text
	global	asm_main
asm_main:
	enter	   0,0			; set up
	pusha				; set up

	mov		eax, msg1	; eax = address of msg1
	call		print_string	; print msg1
	call		read_int	; get an integer from the keyboard (in EAX)
	push		eax		; put the integer on the stack (parameter #1)
	call		recursive_sum	; call recursive_sum
	pop		ebx		; remove the parameter from the stack
	mov		ebx, eax	; save the value returned by recursive_sum
	mov		eax, msg2	; eax = address of msg2
	call		print_string	; print msg2
	mov		eax, ebx	; eax = sum
	call		print_int	; print the sum
	call		print_nl	; print a new line

	popa				; clean up
	mov	   eax, 0		; clean up
	leave				; clean up
	ret				; clean up

;   	FUNCTION: recursive_sum
; 	Takes one parameter:  an integer
; 	Destroys values of eax, ebx, ecx, and edx!! (eax = return value)
segment .bss
	value		resd  1		; to store the return value temporarily

segment .text
recursive_sum:
	push		ebp		; save ebp
	mov		ebp, esp	; set EBP = ESP
	pusha				; save all registers (probably overkill)
	mov		ebx, [ebp+8]	; ebx = integer (parameter #1)
	cmp		ebx, 0		; ebx = 0 ?
	jnz		next		; if (ebx != 0) go to next
	xor		ecx, ecx	; ECX = 0
	jmp		end		; Jump to end
next:
	mov		ecx, ebx	; ECX = EBX
	dec		ecx		; ECX = ECX - 1
	push		ecx		; put ECX on the stack
	call		recursive_sum	; recursive call to recursive_sum!
	pop		edx		; pop the parameter from the stack
	add		ebx, eax	; EBX = EBX + recursive_sum(EBX -1)
	mov		ecx, ebx	; ECX = EBX
end:					; at this point, ECX contains the result
	mov		[value], ecx	; save ECX, the return value, in memory
	popa				; restore all registers
	mov		eax, [value]	; put the saved returned value into eax

	pop		ebp		; restore EBP
	ret				; return


