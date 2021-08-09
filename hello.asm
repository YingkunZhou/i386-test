%define IMM8 0xff
%define IMM16 0xffff
%define IMM32 0xffffffff

default rel
global    _start
section   .text

RET0:
  ret

PUSH0:
  push rax

POP0:
  pop rax

ADD0:
  add rax, 0xffffff

REG0:
  push ax

REG1:
  push rcx

REG2:
  push rdx

REG2_:
  push dx

REG3:
  push rbx

REG4:
  push rsi

REG5:
  push rdi

REG6:
  push rsp

REG7:
  push rbp

REG8:
  push r8

REG9:
  push r9

REG10:
  push r10

REG11:
  push r11

REG12:
  push r12

REG13:
  push r13

REG14:
  push r14

REG15:
  push r15

REG15_:
  push r15w

REGCS:
  mov ax, cs

REGDS:
  mov ax, ds

REGSS:
  mov ax, ss

REGES:
  mov ax, es

REGFS:
  mov ax, fs

REGGS:
  mov ax, gs

PUSHF_:
  pushf

PUSHFQ_:
  pushfq

FLD0:
  fld dword [x]

MMX0:
  movq [ebp], mm0

MOV0:
  mov byte [l1] ,1

MOV1:
  mov word [l1], 1

MOV2:
  mov dword [l1], 1

MOV3:
  mov [l1], eax

MOV4:
  mov [l1], ax

MOV5:
  mov [l1], al

MOV6:
  mov eax, [l1]

MOV7:
  mov ax, [l1]

MOV8:
  mov eax, 12

MOV9:
  mov ax, 12

MOV10:
  mov al, 12

MOV11:
  mov ah, 12




_start:
  mov       rax, 0x2000004          ; system call for write
  mov       rdi, 1                  ; file handle 1 is stdout
  mov       rsi, msg                ; address the string message
  mov       rdx, msg.len            ; number of bytes
  syscall                           ; invoke operating system to do the write

  mov       rax, 0x2000001          ; system call for exit
  mov       rdi, 0                  ; exit code 0
  syscall                           ; invoke operating system to exit

section   .data

 msg:     db        "Hello World!", 10      ; note the newline at the end
.len:     equ        $ - msg

 l0: dd 1.5
 l1: db 00h, 04Fh, 012h, 0A4h
 l2: db 00h, 04Fh, 012h, 0A4h
