	; bios set 1
	.xref event00_main
	.xref event01_main
	.xref event02_main
	; bios set 2
	.xref event10_main
	.xref event11_main
	.xref event12_main

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
	bltz    v0, @@else
	lw      ra, 0x10(sp)
	; --
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
@@else:
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
	;
	lw      v0, 0x7258(zero)
	ori     t7, v1, 0x1003
	sll     t8, a0, 2
	li      t9, 0x3DB4
	lw      t9, 0(t9)
	nop
	jr      t9
	nop
