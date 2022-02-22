	.file	"assemblytest.cpp"
# GNU C++17 (Ubuntu 11.1.0-1ubuntu1~18.04.1) version 11.1.0 (x86_64-linux-gnu)
#	compiled by GNU C version 11.1.0, GMP version 6.1.2, MPFR version 4.0.1, MPC version 1.1.0, isl version isl-0.19-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -march=skylake -mmmx -mpopcnt -msse -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -mavx -mno-sse4a -mno-fma4 -mno-xop -mfma -mno-avx512f -mbmi -mbmi2 -maes -mpclmul -mno-avx512vl -mno-avx512bw -mno-avx512dq -mno-avx512cd -mno-avx512er -mno-avx512pf -mno-avx512vbmi -mno-avx512ifma -mno-avx5124vnniw -mno-avx5124fmaps -mno-avx512vpopcntdq -mno-avx512vbmi2 -mno-gfni -mno-vpclmulqdq -mno-avx512vnni -mno-avx512bitalg -mno-avx512bf16 -mno-avx512vp2intersect -mno-3dnow -madx -mabm -mno-cldemote -mclflushopt -mno-clwb -mno-clzero -mcx16 -mno-enqcmd -mf16c -mfsgsbase -mfxsr -mno-hle -msahf -mno-lwp -mlzcnt -mmovbe -mno-movdir64b -mno-movdiri -mno-mwaitx -mno-pconfig -mno-pku -mno-prefetchwt1 -mprfchw -mno-ptwrite -mno-rdpid -mrdrnd -mrdseed -mno-rtm -mno-serialize -msgx -mno-sha -mno-shstk -mno-tbm -mno-tsxldtrk -mno-vaes -mno-waitpkg -mno-wbnoinvd -mxsave -mxsavec -mxsaveopt -mxsaves -mno-amx-tile -mno-amx-int8 -mno-amx-bf16 -mno-uintr -mno-hreset -mno-kl -mno-widekl -mno-avxvnni --param=l1-cache-size=32 --param=l1-cache-line-size=64 --param=l2-cache-size=4096 -mtune=skylake -mavx2 -Ofast -fPIC -fexpensive-optimizations -funroll-all-loops -fasynchronous-unwind-tables -fstack-protector-strong
	.text
	.p2align 4
	.globl	_ZN5MC64K12AssemblyTest6setGPREv
	.type	_ZN5MC64K12AssemblyTest6setGPREv, @function
_ZN5MC64K12AssemblyTest6setGPREv:
.LFB59:
	.cfi_startproc
# assemblytest.cpp:23:     Interpreter::gpr<0>().value<int8>()   = 0x11;
	movq	_ZN5MC64K7Machine11Interpreter5aoGPRE@GOTPCREL(%rip), %rax	#, tmp82
# assemblytest.cpp:25:     Interpreter::gpr<2>().value<int16>()  = 0x3333;
	movl	$13107, %edx	#,
# assemblytest.cpp:26:     Interpreter::gpr<3>().value<uint16>() = 0x4444;
	movl	$17476, %ecx	#,
# assemblytest.cpp:29:     Interpreter::gpr<6>().value<int64>()  = 0x6666666666666666;
	movabsq	$7378697629483820646, %rsi	#, tmp93
# assemblytest.cpp:30:     Interpreter::gpr<7>().value<uint64>() = 0x7777777777777777;
	movabsq	$8608480567731124087, %rdi	#, tmp94
# assemblytest.cpp:23:     Interpreter::gpr<0>().value<int8>()   = 0x11;
	movb	$17, (%rax)	#, MEM[(signed char &)&aoGPR]
# assemblytest.cpp:24:     Interpreter::gpr<1>().value<uint8>()  = 0x22;
	movb	$34, 8(%rax)	#, MEM[(unsigned char &)&aoGPR + 8]
# assemblytest.cpp:25:     Interpreter::gpr<2>().value<int16>()  = 0x3333;
	movw	%dx, 16(%rax)	#, MEM[(short int &)&aoGPR + 16]
# assemblytest.cpp:26:     Interpreter::gpr<3>().value<uint16>() = 0x4444;
	movw	%cx, 24(%rax)	#, MEM[(short unsigned int &)&aoGPR + 24]
# assemblytest.cpp:27:     Interpreter::gpr<4>().value<int32>()  = 0x55555555;
	movl	$1431655765, 32(%rax)	#, MEM[(int &)&aoGPR + 32]
# assemblytest.cpp:28:     Interpreter::gpr<5>().value<uint32>() = 0x55555555;
	movl	$1431655765, 40(%rax)	#, MEM[(unsigned int &)&aoGPR + 40]
# assemblytest.cpp:29:     Interpreter::gpr<6>().value<int64>()  = 0x6666666666666666;
	movq	%rsi, 48(%rax)	# tmp93, MEM[(long int &)&aoGPR + 48]
# assemblytest.cpp:30:     Interpreter::gpr<7>().value<uint64>() = 0x7777777777777777;
	movq	%rdi, 56(%rax)	# tmp94, MEM[(long unsigned int &)&aoGPR + 56]
# assemblytest.cpp:31: }
	ret	
	.cfi_endproc
.LFE59:
	.size	_ZN5MC64K12AssemblyTest6setGPREv, .-_ZN5MC64K12AssemblyTest6setGPREv
	.p2align 4
	.globl	_ZN5MC64K12AssemblyTest6setFPREv
	.type	_ZN5MC64K12AssemblyTest6setFPREv, @function
_ZN5MC64K12AssemblyTest6setFPREv:
.LFB68:
	.cfi_startproc
# assemblytest.cpp:34:     Interpreter::fpr<0>().value<float32>()  = 0.25;
	movq	_ZN5MC64K7Machine11Interpreter5aoFPRE@GOTPCREL(%rip), %rax	#, tmp82
# assemblytest.cpp:35:     Interpreter::fpr<1>().value<float64>()  = 0.125;
	movq	.LC1(%rip), %rdx	#, tmp87
# assemblytest.cpp:34:     Interpreter::fpr<0>().value<float32>()  = 0.25;
	movl	$0x3e800000, (%rax)	#, MEM[(float &)&aoFPR]
# assemblytest.cpp:35:     Interpreter::fpr<1>().value<float64>()  = 0.125;
	movq	%rdx, 8(%rax)	# tmp87, MEM[(double &)&aoFPR + 8]
# assemblytest.cpp:36: }
	ret	
	.cfi_endproc
.LFE68:
	.size	_ZN5MC64K12AssemblyTest6setFPREv, .-_ZN5MC64K12AssemblyTest6setFPREv
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1069547520
	.ident	"GCC: (Ubuntu 11.1.0-1ubuntu1~18.04.1) 11.1.0"
	.section	.note.GNU-stack,"",@progbits
