
; cop0 register aliases
Wired:		equ $3		; BPC - Breakpoint on execute (R/W)
PageMask:	equ $5		; BDA - Breakpoint on data access (R/W)
EntryLo1:	equ $6		; JUMPDEST - Randomly memorized jump address (R)
DCIC:		equ $7		; DCIC - Breakpoint control (R/W)
BVA:		equ $8		; BadVaddr - Bad Virtual Address (R)
Count:		equ $9		; BDAM - Data Access breakpoint mask (R/W)
Compare:	equ $11		; BPCM - Execute breakpoint mask (R/W)
SR:			equ $12		; System status register (R/W)
Cause:		equ $13		; (R)  Describes the most recently recognised exception
EPC:		equ $14		; Return Address from Trap (R)
PRID:		equ $15		; Processor ID (R)

set_boot:
	mtc0 r0, DCIC
	nop
	mtc0 a0, PageMask
	nop
	mtc0 a0, EntryLo1
	nop
	mtc0 a0, Wired
	nop
	mtc0 a1, Count
	nop
	mtc0 a1, Compare
	nop
	mtc0 a2, DCIC
	nop
	jr ra
	nop

reset_DCIC:
	mtc0 r0, DCIC
	nop
	jr ra
	nop

get_SR:
	mfc0 v0, SR
	nop
	jr ra
	nop

set_SR:
	mtc0 a0, SR
	nop
	jr ra
	nop

get_Cause:
	mfc0 v0, Cause
	nop
	jr ra
	nop

get_EPC:
	mfc0 v0, EPC
	nop
	jr ra
	nop

init_cache:
	la t0, (init_cache_kuseg & 0xFFFFFFF) | 0xB0000000	; turn function to kuseg
	jr t0
	nop

init_cache_kuseg:
	li t1, 0xFFFE0130
	lw t3, 0(t1)
	li t4, 0xFFFFFB7C
	and t4, t3
	ori t4, 0x84		; enable scratchpad
	sw t4, 0(t1)
	mfc0 t2, SR
	lui at, 1
	or at, t2
	mtc0 at, SR
	nop
	nop
	li t4, 0
	li t0, 0xF80
@memzero:
	sw r0, 0(t4)
	sw r0, 0x10(t4)
	sw r0, 0x20(t4)
	sw r0, 0x30(t4)
	sw r0, 0x40(t4)
	sw r0, 0x50(t4)
	sw r0, 0x60(t4)
	sw r0, 0x70(t4)
	bne t4, t0, @memzero
	addiu t4, 128
	;
	mtc0 t2, SR
	nop
	nop
	sw t3, 0(t1)
	nop
	nop
	jr ra
	nop	