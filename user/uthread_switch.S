	.text

	/*
         * save the old thread's registers,
         * restore the new thread's registers.
         */

	.globl thread_switch
thread_switch:
	/* YOUR CODE HERE */
	lui t2,0x2
	addi t2,t2,0x8
	add t0,a0,t2
	sd ra,-0x8(sp)
	sd sp,0x0(t0)
	sd s0,0x8(t0)
	sd s1,0x10(t0)
	sd s2,0x18(t0)
	sd s3,0x20(t0)
	sd s4,0x28(t0)
	sd s5,0x30(t0)
	sd s6,0x38(t0)
	sd s7,0x40(t0)
	sd s8,0x48(t0)
	sd s9,0x50(t0)
	sd s10,0x58(t0)
	sd s11,0x60(t0)
	add t1,a1,t2
	ld sp,0x0(t1)
	ld s0,0x8(t1)
	ld s1,0x10(t1)
	ld s2,0x18(t1)
	ld s3,0x20(t1)
	ld s4,0x28(t1)
	ld s5,0x30(t1)
	ld s6,0x38(t1)
	ld s7,0x40(t1)
	ld s8,0x48(t1)
	ld s9,0x50(t1)
	ld s10,0x58(t1)
	ld s11,0x60(t1)
	ld ra,-0x8(sp)
	ret    /* return to ra */
