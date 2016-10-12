#include <r3000.h>
#include <asm.h>

#define r0	zero

	.text
	.set    noreorder

	// bios set 1
	.globl install_event00
	.globl install_event01
	.globl install_event02
	// bios set 2
	.globl install_event10
	.globl install_event11
	.globl install_event12

install_event00:
	move t0, a0
	la t1, jmp_event00
	lw t2, 0(t1)
	lw t3, 4(t1)
	lw t4, 8(t1)
	lw t5, 0xC(t1)
	sw t2, 0(t0)
	sw t3, 4(t0)
	sw t4, 8(t0)
	jr ra
	sw t5, 0xC(t0)

jmp_event00:
	la t9, event00
	jr t9
	nop

event00:
	addiu   sp, -0x54
	sw      ra, 0x10(sp)
	sw      v0, 0x14(sp)
	sw      v1, 0x18(sp)
	sw      a0, 0x1C(sp)
	sw      a1, 0x20(sp)
	sw      a2, 0x24(sp)
	sw      a3, 0x28(sp)
	sw      t0, 0x2C(sp)
	sw      t1, 0x30(sp)
	sw      t2, 0x34(sp)
	sw      t3, 0x38(sp)
	sw      t4, 0x3C(sp)
	sw      t5, 0x40(sp)
	sw      t6, 0x44(sp)
	sw      t7, 0x48(sp)
	sw      t8, 0x4C(sp)
	sw      t9, 0x50(sp)
	jal     event00_main
	nop
	bltz    v0, ELSE00
	lw      ra, 0x10(sp)
	// --
	lw      v0, 0x14(sp)
	lw      v1, 0x18(sp)
	lw      a0, 0x1C(sp)
	lw      a1, 0x20(sp)
	lw      a2, 0x24(sp)
	lw      a3, 0x28(sp)
	lw      t0, 0x2C(sp)
	lw      t1, 0x30(sp)
	lw      t2, 0x34(sp)
	lw      t3, 0x38(sp)
	lw      t4, 0x3C(sp)
	lw      t5, 0x40(sp)
	lw      t6, 0x44(sp)
	lw      t7, 0x48(sp)
	lw      t8, 0x4C(sp)
	lw      t9, 0x50(sp)
	addiu   sp, 0x54
	li      v0, 1
	jr      ra
	addiu   sp, 0x18
ELSE00:
	lw      v0, 0x14(sp)
	lw      v1, 0x18(sp)
	lw      a0, 0x1C(sp)
	lw      a1, 0x20(sp)
	lw      a2, 0x24(sp)
	lw      a3, 0x28(sp)
	lw      t0, 0x2C(sp)
	lw      t1, 0x30(sp)
	lw      t2, 0x34(sp)
	lw      t3, 0x38(sp)
	lw      t4, 0x3C(sp)
	lw      t5, 0x40(sp)
	lw      t6, 0x44(sp)
	lw      t7, 0x48(sp)
	lw      t8, 0x4C(sp)
	lw      t9, 0x50(sp)
	addiu   sp, 0x54
	//
	lw      v0, 0x7258(r0)
	ori     t7, v1, 0x1003
	sll     t8, a0, 2
	lw      t9, 0x3DB4(r0)
	nop
	jr      t9
	nop

install_event01:
	move t0, a0
	la t1, jmp_event01
	lw t2, 0(t1)
	lw t3, 4(t1)
	lw t4, 8(t1)
	lw t5, 0xC(t1)
	sw t2, 0(t0)
	sw t3, 4(t0)
	sw t4, 8(t0)
	jr ra
	sw t5, 0xC(t0)

jmp_event01:
	la t9, event01
	jr t9
	nop

event01:
	addiu   sp, -0x54
	sw      ra, 0x10(sp)
	sw      v0, 0x14(sp)
	sw      v1, 0x18(sp)
	sw      a0, 0x1C(sp)
	sw      a1, 0x20(sp)
	sw      a2, 0x24(sp)
	sw      a3, 0x28(sp)
	sw      t0, 0x2C(sp)
	sw      t1, 0x30(sp)
	sw      t2, 0x34(sp)
	sw      t3, 0x38(sp)
	sw      t4, 0x3C(sp)
	sw      t5, 0x40(sp)
	sw      t6, 0x44(sp)
	sw      t7, 0x48(sp)
	sw      t8, 0x4C(sp)
	sw      t9, 0x50(sp)
	jal     event01_main
	nop
	bltz    v0, ELSE01
	nop
	lw      ra, 0x10(sp)
	lw      v0, 0x14(sp)
	lw      v1, 0x18(sp)
	lw      a0, 0x1C(sp)
	lw      a1, 0x20(sp)
	lw      a2, 0x24(sp)
	lw      a3, 0x28(sp)
	lw      t0, 0x2C(sp)
	lw      t1, 0x30(sp)
	lw      t2, 0x34(sp)
	lw      t3, 0x38(sp)
	lw      t4, 0x3C(sp)
	lw      t5, 0x40(sp)
	lw      t6, 0x44(sp)
	lw      t7, 0x48(sp)
	lw      t8, 0x4C(sp)
	lw      t9, 0x50(sp)
	addiu   sp, 0x54
	li      v0, 1
	jr      ra
	addiu   sp, 0x18
ELSE01:
	lw      ra, 0x10(sp)
	lw      v0, 0x14(sp)
	lw      v1, 0x18(sp)
	lw      a0, 0x1C(sp)
	lw      a1, 0x20(sp)
	lw      a2, 0x24(sp)
	lw      a3, 0x28(sp)
	lw      t0, 0x2C(sp)
	lw      t1, 0x30(sp)
	lw      t2, 0x34(sp)
	lw      t3, 0x38(sp)
	lw      t4, 0x3C(sp)
	lw      t5, 0x40(sp)
	lw      t6, 0x44(sp)
	lw      t7, 0x48(sp)
	lw      t8, 0x4C(sp)
	lw      t9, 0x50(sp)
	addiu   sp, 0x54
	lw      v1, 0x7258(zero)
	ori     t9, v0, 0x1003
	sll     t0, a3, 2
	li      t1, 0x3DB4
	lw      t1, 4(t1)
	nop
	jr      t1
	nop

install_event02:
	move t0, a0
	la t1, jmp_event02
	lw t2, 0(t1)
	lw t3, 4(t1)
	lw t4, 8(t1)
	lw t5, 0xC(t1)
	sw t2, 0(t0)
	sw t3, 4(t0)
	sw t4, 8(t0)
	jr ra
	sw t5, 0xC(t0)

jmp_event02:
	la t9, event02
	jr t9
	nop

event02:
	addiu   sp, -0x54
	sw      ra, 0x10(sp)
	sw      v0, 0x14(sp)
	sw      v1, 0x18(sp)
	sw      a0, 0x1C(sp)
	sw      a1, 0x20(sp)
	sw      a2, 0x24(sp)
	sw      a3, 0x28(sp)
	sw      t0, 0x2C(sp)
	sw      t1, 0x30(sp)
	sw      t2, 0x34(sp)
	sw      t3, 0x38(sp)
	sw      t4, 0x3C(sp)
	sw      t5, 0x40(sp)
	sw      t6, 0x44(sp)
	sw      t7, 0x48(sp)
	sw      t8, 0x4C(sp)
	sw      t9, 0x50(sp)
	jal     event02_main
	nop
	bltz    v0, ELSE02
	nop
	lw      ra, 0x10(sp)
	lw      v0, 0x14(sp)
	lw      v1, 0x18(sp)
	lw      a0, 0x1C(sp)
	lw      a1, 0x20(sp)
	lw      a2, 0x24(sp)
	lw      a3, 0x28(sp)
	lw      t0, 0x2C(sp)
	lw      t1, 0x30(sp)
	lw      t2, 0x34(sp)
	lw      t3, 0x38(sp)
	lw      t4, 0x3C(sp)
	lw      t5, 0x40(sp)
	lw      t6, 0x44(sp)
	lw      t7, 0x48(sp)
	lw      t8, 0x4C(sp)
	lw      t9, 0x50(sp)
	addiu   sp, 0x54
	li      v0, 1
	jr      ra
	addiu   sp, 0x20
ELSE02:
	lw      ra, 0x10(sp)
	lw      v0, 0x14(sp)
	lw      v1, 0x18(sp)
	lw      a0, 0x1C(sp)
	lw      a1, 0x20(sp)
	lw      a2, 0x24(sp)
	lw      a3, 0x28(sp)
	lw      t0, 0x2C(sp)
	lw      t1, 0x30(sp)
	lw      t2, 0x34(sp)
	lw      t3, 0x38(sp)
	lw      t4, 0x3C(sp)
	lw      t5, 0x40(sp)
	lw      t6, 0x44(sp)
	lw      t7, 0x48(sp)
	lw      t8, 0x4C(sp)
	lw      t9, 0x50(sp)
	addiu   sp, 0x54
	lw      a0, 0x7258(zero)
	ori     t8, v0, 0x1003
	sll     t0, a2, 2
	li      t1, 0x3DB4
	lw      t1, 8(t1)
	nop
	jr      t1
	nop

install_event10:
install_event11:
install_event12:
