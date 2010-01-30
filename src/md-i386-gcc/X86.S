/* These are optimized x86 assembly versions of pfield_linetoscr.
 * Feel free to send me Sparc/PPC/Alpha versions of this... :)
 * [it's not necessarily a win to code these in assembler, though - Paul
 * Liss says this code is slower than the generic C stuff in custom.c on
 * a PPro]
 */

#include "config.h"

/*#define X86_PPRO_OPT*/
#ifdef X86_PPRO_OPT
#define PARTIAL_REG(a,b) a
#define BYTE_MOVE movzbl
#define WORD_MOVE movzwl
#define CLEAR_FOR_BYTE_MOVE(a)
#else
#define PARTIAL_REG(a,b) b
#define BYTE_MOVE movb
#define WORD_MOVE movw
#define CLEAR_FOR_BYTE_MOVE(a) xorl a,a
#endif

#ifndef USE_UNDERSCORE
#define SYM(NAME) NAME
#define FUNCTION_ALIGN .align 16
#define FUNCTYPE(NAME) .type NAME,@function
#else
#define SYM(NAME) _##NAME
#define FUNCTION_ALIGN .align 4
#define FUNCTYPE(NAME)
#endif

 	.text

        .globl DitherLine
#ifndef USE_UNDERSCORE
	.type	 DitherLine,@function
#endif
	FUNCTION_ALIGN
DitherLine:
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 20(%esp),%edi
	xorl %ebx,%ebx
	movw 36(%esp),%bx
	movl 32(%esp),%edx
	andl $3,%edx
	sall $15,%edx
	movl 28(%esp),%eax
	andl $3,%eax
	sall $12,%eax
	leal SYM(cidx)(%edx,%eax),%ebp
	xorb %dl,%dl
	movl $8,%ecx
	testl %ebx,%ebx
	je .Li_end
	cmpl $8,40(%esp)
	je .Li_fast

	movl 24(%esp),%esi
.Li_loop:
	movzwl (%esi),%eax
	movzbl (%eax,%ebp),%eax
	subl 40(%esp),%ecx
	sall %cl,%eax
	orb %al,%dl
	testl %ecx,%ecx
	jne .Li_1
	movb %dl,(%edi)
	incl %edi
	movl $8,%ecx
	xorb %dl,%dl
.Li_1:
	movzwl 2(%esi),%eax
	movzbl 4096(%ebp,%eax),%eax
	subl 40(%esp),%ecx
	sall %cl,%eax
	orb %al,%dl
	testl %ecx,%ecx
	jne .Li_2
	movb %dl,(%edi)
	incl %edi
	movl $8,%ecx
	xorb %dl,%dl
.Li_2:
	movzwl 4(%esi),%eax
	movzbl 8192(%ebp,%eax),%eax
	subl 40(%esp),%ecx
	sall %cl,%eax
	orb %al,%dl
	testl %ecx,%ecx
	jne .Li_3
	movb %dl,(%edi)
	incl %edi
	movl $8,%ecx
	xorb %dl,%dl
.Li_3:
	movzwl 6(%esi),%eax
	movzbl 12288(%ebp,%eax),%eax
	addl $8,%esi
	subl 40(%esp),%ecx
	sall %cl,%eax
	orb %al,%dl
	testl %ecx,%ecx
	jne .Li_4
	movb %dl,(%edi)
	incl %edi
	movl $8,%ecx
	xorb %dl,%dl
.Li_4:
	subl $4,%ebx
	jne .Li_loop
	jmp .Li_end
	
	/* Fast 8-bit version */
.Li_fast:
	movl 24(%esp),%esi
	xorl %edx,%edx
	xorl %ecx,%ecx
	FUNCTION_ALIGN
.Li_fast_loop:
	movw (%esi),%dx
	movw 2(%esi),%cx
	movb (%edx,%ebp),%al	
	movw 4(%esi),%dx
	movb 4096(%ebp,%ecx),%ah
	
	movw 6(%esi),%cx
	sall $16,%eax
	movb 8192(%ebp,%edx),%al
	
	movb 12288(%ebp,%ecx),%ah
	
	roll $16,%eax
	movl %eax,(%edi)
	addl $4,%edi
	addl $8,%esi
	
	subl $4,%ebx
	jne .Li_fast_loop

.Li_end:
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	ret
#if 0	
.globl compiler_do_rts
        /* Entry: EDX == regs.regs + 15 */
compiler_do_rts:
        movl (%edx),%esi
        addl address_space,%esi
	movl jsr_num,%ecx
	movl (%esi),%esi
	orl %ecx,%ecx
	bswap %esi
	jz cdrts_noway
	decl %ecx
	cmpl jsr_rets(,%ecx,4),%esi
	jne cdrts_noway
	movl jsr_hash(,%ecx,4),%ebx
	movl (%ebx),%ebx
	orl %ebx,%ebx
	jz cdrts_noway
	addl $4,(%edx)
	movl %ecx,jsr_num
	jmp %ebx
cdrts_noway:
        ret
#endif
#if 0
.globl longget_stub
longget_stub:
        pushl %ecx
        pushl %eax
	pushl %edx
	call SYM(longget)
	popl %ecx
	movl %eax,%ecx
	popl %eax
        ret

.globl wordget_stub
wordget_stub:
        pushl %ecx
        pushl %eax
	pushl %edx
	call SYM(wordget)
	popl %ecx
	movl %eax,%ecx
	popl %eax
        ret

.globl byteget_stub
byteget_stub:
        pushl %ecx
        pushl %eax
	pushl %edx
	call SYM(byteget)
	popl %ecx
	movl %eax,%ecx
	popl %eax
        ret

.globl longput_stub
longput_stub:
        pushl %ecx
        pushl %eax
	pushl %ebx
	pushl %edx
	call SYM(longput)
	movl %eax,%ecx
	addl $8,%esp
	popl %eax
        ret

.globl wordput_stub
wordput_stub:
        pushl %ecx
        pushl %eax
	pushl %ebx
	pushl %edx
	call SYM(wordput)
	movl %eax,%ecx
	addl $8,%esp
	popl %eax
        ret

.globl byteput_stub
byteput_stub:
        pushl %ecx
        pushl %eax
	pushl %ebx
	pushl %edx
	call SYM(byteput)
	movl %eax,%ecx
	addl $8,%esp
	popl %eax
        ret
#endif
