    opt     c+

	section	.rdata
	section	.ctors
	section	.dtors
	section	.sdata
	section	.sbss
	section	.bss

 	section	.data

; externs for SN entry redefinition
	xdef	__SN_ENTRY_POINT,__main
	xdef	__heapbase,__heapsize
	xdef	__text,__textlen
	xdef	__data,__datalen
	xdef	__bss,__bsslen
	xdef	__packable
	xref	main

; a couple useful functions
	global Sp_to_scratch
	global Sp_restore
	global Reboot

	global receive
	global bank
	global port
	global w
	global k

receive:	equ 0x1f806010
bank:		equ 0x1f006030
port:		equ 0x1f006038
w:			equ 0x3DB4
k:			equ 0x0

;====================================================
;= Entry point function with less fuss				=
;====================================================
	section	.text

__SN_ENTRY_POINT:
stup1:
stup2:
	la	  t0, sect(.sbss)
	la	  t1, sectend(.bss)
@clrit:						; clear bss
	opt	at-
	sw    r0, 0(t0)			; write to bottom of area to clear
	addiu t0, t0, 4			; increment bottom
	sltu  at, t0, t1		; at = bottom - top
	bnez  at, @clrit		; if at is nonzero, loop
	nop
	opt	at+

	la	  gp, sect(.sdata)
	la    sp, 0x80200000
	move  fp, sp
stup0:
	j	  main
	nop

__main:
	jr	  ra
	nop

;====================================================
;= Stack pointer to scratchpad / restore code		=
;====================================================
Sp_to_scratch:
	la t0, 0x1f800400	; scratchpad address
	sw sp, -4(t0)		; save stack pointer
	jr ra
	addiu sp, t0, -8	; assign new stack

Sp_restore:
	lui t0, 0x1f80
	jr ra
	lw sp, 0x0400-4(t0)	; restore old stack

Reboot:
	jr a0
	nop

;====================================================
;= Redefine some SN variables for entry point		=
;====================================================
	section	.data

	cnop	0,4				; longword align

_stacksize	dw	$00008000	; default stack is 32k
_ramsize	dw	$00200000	; 2 Megabytes
__text		dw	sect(.text)
__textlen	dw	sectend(.text)-sect(.text)
__data		dw	sect(.data)
__datalen	dw	sectend(.data)-sect(.data)
__bss		dw	sect(.bss)
__bsslen	dw	sectend(.bss)-sect(.bss)

__packable	dw	sectend(.bss)		; we can use this as an incremental allocator

;====================================================
;= Data inclusion for external resources			=
;====================================================

	section	.sbss

__ra_temp	dsw	1
	end
