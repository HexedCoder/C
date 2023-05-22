	.file	"factorial.c"
	.intel_syntax noprefix
# GNU C17 (Ubuntu 9.5.0-1ubuntu1~22.04) version 9.5.0 (x86_64-linux-gnu)
#	compiled by GNU C version 9.5.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed:  -imultiarch x86_64-linux-gnu factorial.c -masm=intel
# -mtune=generic -march=x86-64 -auxbase-strip factorial.s -fverbose-asm
# -fasynchronous-unwind-tables -fstack-protector-strong -Wformat
# -Wformat-security -fstack-clash-protection -fcf-protection
# options enabled:  -fPIC -fPIE -faggressive-loop-optimizations
# -fassume-phsa -fasynchronous-unwind-tables -fauto-inc-dec -fcommon
# -fdelete-null-pointer-checks -fdwarf2-cfi-asm -fearly-inlining
# -feliminate-unused-debug-types -ffp-int-builtin-inexact -ffunction-cse
# -fgcse-lm -fgnu-runtime -fgnu-unique -fident -finline-atomics
# -fipa-stack-alignment -fira-hoist-pressure -fira-share-save-slots
# -fira-share-spill-slots -fivopts -fkeep-static-consts
# -fleading-underscore -flifetime-dse -flto-odr-type-merging -fmath-errno
# -fmerge-debug-strings -fpeephole -fplt -fprefetch-loop-arrays
# -freg-struct-return -fsched-critical-path-heuristic
# -fsched-dep-count-heuristic -fsched-group-heuristic -fsched-interblock
# -fsched-last-insn-heuristic -fsched-rank-heuristic -fsched-spec
# -fsched-spec-insn-heuristic -fsched-stalled-insns-dep -fschedule-fusion
# -fsemantic-interposition -fshow-column -fshrink-wrap-separate
# -fsigned-zeros -fsplit-ivs-in-unroller -fssa-backprop
# -fstack-clash-protection -fstack-protector-strong -fstdarg-opt
# -fstrict-volatile-bitfields -fsync-libcalls -ftrapping-math -ftree-cselim
# -ftree-forwprop -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
# -ftree-loop-optimize -ftree-parallelize-loops= -ftree-phiprop
# -ftree-reassoc -ftree-scev-cprop -funit-at-a-time -funwind-tables
# -fverbose-asm -fzero-initialized-in-bss -m128bit-long-double -m64 -m80387
# -malign-stringops -mavx256-split-unaligned-load
# -mavx256-split-unaligned-store -mfancy-math-387 -mfp-ret-in-387 -mfxsr
# -mglibc -mieee-fp -mlong-double-80 -mmmx -mno-sse4 -mpush-args -mred-zone
# -msse -msse2 -mstv -mtls-direct-seg-refs -mvzeroupper

	.text
	.globl	factorial
	.type	factorial, @function
factorial:
.LFB6:
	.cfi_startproc
	endbr64	
	push	rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp	#,
	.cfi_def_cfa_register 6
	sub	rsp, 16	#,
	mov	DWORD PTR -4[rbp], edi	# num, num
# factorial.c:26:     if (num < 2){
	cmp	DWORD PTR -4[rbp], 1	# num,
	jg	.L2	#,
# factorial.c:27:         return 1;
	mov	eax, 1	# _3,
	jmp	.L3	#
.L2:
# factorial.c:30:     return num * factorial(num - 1);
	mov	eax, DWORD PTR -4[rbp]	# tmp86, num
	sub	eax, 1	# _1,
	mov	edi, eax	#, _1
	call	factorial	#
# factorial.c:30:     return num * factorial(num - 1);
	imul	eax, DWORD PTR -4[rbp]	# _3, num
.L3:
# factorial.c:32: }
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE6:
	.size	factorial, .-factorial
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	endbr64	
	push	rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp	#,
	.cfi_def_cfa_register 6
	sub	rsp, 32	#,
	mov	DWORD PTR -20[rbp], edi	# argc, argc
	mov	QWORD PTR -32[rbp], rsi	# argv, argv
# factorial.c:36:     if (argc != 2) {
	cmp	DWORD PTR -20[rbp], 2	# argc,
	je	.L5	#,
# factorial.c:37:         return 1;
	mov	eax, 1	# _4,
	jmp	.L6	#
.L5:
# factorial.c:40:     int num = atoi(argv[1]);
	mov	rax, QWORD PTR -32[rbp]	# tmp87, argv
	add	rax, 8	# _1,
# factorial.c:40:     int num = atoi(argv[1]);
	mov	rax, QWORD PTR [rax]	# _2, *_1
	mov	rdi, rax	#, _2
	call	atoi@PLT	#
	mov	DWORD PTR -4[rbp], eax	# num, tmp88
# factorial.c:42:     printf("%d\n", factorial(num));
	mov	eax, DWORD PTR -4[rbp]	# tmp89, num
	mov	edi, eax	#, tmp89
	call	factorial	#
	mov	esi, eax	#, _3
	lea	rdi, .LC0[rip]	#,
	mov	eax, 0	#,
	call	printf@PLT	#
	mov	eax, 0	# _4,
.L6:
# factorial.c:44: }				/* main() */
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.5.0-1ubuntu1~22.04) 9.5.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
