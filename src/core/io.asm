#include <r3000.h>
#include <asm.h>

	.text
	.set    noreorder

	.globl set_boot

// cop0 register aliases
#define Wired		R3		// BPC - Breakpoint on execute (R/W)
#define PageMask	R5		// BDA - Breakpoint on data access (R/W)
#define EntryLo1	R6		// JUMPDEST - Randomly memorized jump address (R)
#define DCIC		R7		// DCIC - Breakpoint control (R/W)
#define BVA			R8		// BadVaddr - Bad Virtual Address (R)
#define Count		R9		// BDAM - Data Access breakpoint mask (R/W)
#define Compare		R11		// BPCM - Execute breakpoint mask (R/W)
#define SR			R12		// System status register (R/W)
#define Cause		R13		// (R)  Describes the most recently recognised exception
#define EPC			R14		// Return Address from Trap (R)
#define PRID		R15		// Processor ID (R)

set_boot:
	mtc0 zero, DCIC
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
	mtc0 zero, DCIC
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
	la t0, (init_cache_kuseg & 0xFFFFFFF) | 0xB0000000	// turn function to kuseg
	jr t0
	nop

init_cache_kuseg:
	li t1, 0xFFFE0130
	lw t3, 0(t1)
	li t4, 0xFFFFFB7C
	and t4, t3
	ori t4, 0x84		// enable scratchpad
	sw t4, 0(t1)
	mfc0 t2, SR
	lui t0, 1
	or t0, t2
	mtc0 t0, SR
	nop
	nop
	li t4, 0
	li t0, 0xF80
MEMZERO:
	sw zero, 0(t4)
	sw zero, 0x10(t4)
	sw zero, 0x20(t4)
	sw zero, 0x30(t4)
	sw zero, 0x40(t4)
	sw zero, 0x50(t4)
	sw zero, 0x60(t4)
	sw zero, 0x70(t4)
	bne t4, t0, MEMZERO
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