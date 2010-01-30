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

        .globl pfield_linetoscr_asm8
 	FUNCTYPE (pfield_linetoscr_asm8)
	FUNCTION_ALIGN
pfield_linetoscr_asm8:
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 24(%esp),%esi /* start */
	movl 28(%esp),%edi /* lframe_end */
	
	movl SYM(colors_for_drawing),%eax
        movl 20(%esp),%ebx
	movl SYM(xlinebuffer),%ecx
	jmp .La_do_start_line
	FUNCTION_ALIGN
.La_start_line:
	movl %eax,(%ecx,%esi)
	addl $4,%esi
.La_do_start_line:
	cmpl %esi,%edi
	jnbe .La_start_line

	call *%ebx

	/* Finish off the line */

	movl SYM(xlinebuffer),%ecx
        movl 36(%esp),%edi /* stoppos */
	movl SYM(colors_for_drawing),%eax
	jmp .La_do_end_line
	FUNCTION_ALIGN
.La_end_line:
	movl %eax,(%ecx,%esi)
	addl $4,%esi
.La_do_end_line:
	cmpl %esi,%edi
	jnbe .La_end_line

	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	ret

        .globl pfield_linetoscr_ham6_asm8
	FUNCTYPE (pfield_linetoscr_ham6_asm8)
	FUNCTION_ALIGN
pfield_linetoscr_ham6_asm8:
	movl 36(%esp),%edi /* diw_end */
.Laa_hamloop:
	cmpl %esi,%edi
	jbe .Laa_hamend

	movl SYM(ham_linebuf)+12(,%esi,4),%eax
	movb SYM(xcolors)(,%eax,4),%bh
	movl SYM(ham_linebuf)+8(,%esi,4),%eax
	movb SYM(xcolors)(,%eax,4),%bl
	sall $16,%ebx
	movl SYM(ham_linebuf)+4(,%esi,4),%eax
	movb SYM(xcolors)(,%eax,4),%bh
	movl SYM(ham_linebuf)(,%esi,4),%eax
	movb SYM(xcolors)(,%eax,4),%bl
	movl %ebx,(%ecx,%esi)
	addl $4,%esi
	jmp .Laa_hamloop

.Laa_hamend:
        ret

        .globl pfield_linetoscr_dualpf_asm8
	FUNCTYPE (pfield_linetoscr_dualpf_asm8)
	FUNCTION_ALIGN
pfield_linetoscr_dualpf_asm8:
	xorl %edx,%edx
	movl 44(%esp),%ebp /* lookup_no */
.Lab_dp_loop:
        movl 36(%esp),%eax /* diw_end */
	xorl %edx,%edx
	cmpl %esi,%eax
	jbe .La_dp_done

	movl SYM(pixdata)(%esi),%eax

	BYTE_MOVE %al,PARTIAL_REG(%edx,%dl)
	BYTE_MOVE (%ebp,%edx,4),PARTIAL_REG(%edx,%dl)
	movb SYM(colors_for_drawing)(,%edx,4),%cl
	BYTE_MOVE %ah,PARTIAL_REG(%edx,%dl)
	BYTE_MOVE (%ebp,%edx,4),PARTIAL_REG(%edx,%dl)
	movb SYM(colors_for_drawing)(,%edx,4),%ch
	bswap %eax
	bswap %ecx
	BYTE_MOVE %al,PARTIAL_REG(%edx,%dl)
	BYTE_MOVE (%ebp,%edx,4),PARTIAL_REG(%edx,%dl)
	movb SYM(colors_for_drawing)(,%edx,4),%cl
	BYTE_MOVE %ah,PARTIAL_REG(%edx,%dl)
	BYTE_MOVE (%ebp,%edx,4),PARTIAL_REG(%edx,%dl)
	movb SYM(colors_for_drawing)(,%edx,4),%ch
	bswap %ecx
	
	movl SYM(xlinebuffer),%eax
	movl %ecx,(%eax,%esi)
	addl $4,%esi
	jmp .Lab_dp_loop	
.La_dp_done:
	ret

        .globl pfield_linetoscr_ehb_asm8
	FUNCTYPE (pfield_linetoscr_ehb_asm8)
	FUNCTION_ALIGN
pfield_linetoscr_ehb_asm8:
	xorl %edx,%edx
	movl SYM(xlinebuffer),%ebx
.La_ehb_loop:
	movl 36(%esp),%edi /* diw_end */
	cmpl %esi,%edi
	jbe .La_ehb_end
	
	movl SYM(pixdata)(%esi),%eax
	BYTE_MOVE %al,PARTIAL_REG(%edx,%dl)
	cmpb $32,%al
	movb SYM(colors_for_drawing)(,%edx,4),%cl
	jc   .La_ehb1
	movw SYM(colors_for_drawing)+32*4-32*2(,%edx,2),%di
	shrl $1,%edi
	andl $0x777,%edi
	movb SYM(xcolors)(,%edi,4),%cl
.La_ehb1:
	BYTE_MOVE %ah,PARTIAL_REG(%edx,%dl)
	cmpb $32,%ah
	movb SYM(colors_for_drawing)(,%edx,4),%ch
	jc   .La_ehb2
	movw SYM(colors_for_drawing)+32*4-32*2(,%edx,2),%di
	shrl $1,%edi
	andl $0x777,%edi
	movb SYM(xcolors)(,%edi,4),%ch
.La_ehb2:
	sarl $16,%eax
	sall $16,%ecx
	BYTE_MOVE %al,PARTIAL_REG(%edx,%dl)
	cmpb $32,%al
	movb SYM(colors_for_drawing)(,%edx,4),%cl
	jc   .La_ehb3
	movw SYM(colors_for_drawing)+32*4-32*2(,%edx,2),%di
	shrl $1,%edi
	andl $0x777,%edi
	movb SYM(xcolors)(,%edi,4),%cl
.La_ehb3:
        BYTE_MOVE %ah,PARTIAL_REG(%edx,%dl)
	cmpb $32,%ah
	movb SYM(colors_for_drawing)(,%edx,4),%ch
	jc   .La_ehb4
	movw SYM(colors_for_drawing)+32*4-32*2(,%edx,2),%di
	shrl $1,%edi
	andl $0x777,%edi
	movb SYM(xcolors)(,%edi,4),%ch
.La_ehb4:
	roll $16,%ecx

	movl %ecx,(%ebx,%esi)
	addl $4,%esi
	jmp .La_ehb_loop

.La_ehb_end:
        ret

        .globl pfield_linetoscr_normal_asm8
	FUNCTYPE (pfield_linetoscr_normal_asm8)
	FUNCTION_ALIGN
pfield_linetoscr_normal_asm8:
	xorl %edx,%edx
	xorl %ebx,%ebx
	movl 36(%esp),%edi /* diw_end */
	movl SYM(xlinebuffer),%ebp
	jmp .La_normal_doloop
	FUNCTION_ALIGN
.La_normal_loop:
        BYTE_MOVE SYM(pixdata)(%esi),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE SYM(pixdata)+4(%esi),PARTIAL_REG(%ebx,%bl)
	movb SYM(colors_for_drawing)(,%edx,4),%cl
	movb SYM(colors_for_drawing)(,%ebx,4),%al
	BYTE_MOVE SYM(pixdata)+1(%esi),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE SYM(pixdata)+5(%esi),PARTIAL_REG(%ebx,%bl)
	movb SYM(colors_for_drawing)(,%edx,4),%ch
	movb SYM(colors_for_drawing)(,%ebx,4),%ah

	sall $16,%ecx
	BYTE_MOVE SYM(pixdata)+2(%esi),PARTIAL_REG(%edx,%dl)
	sall $16,%eax

	BYTE_MOVE SYM(pixdata)+6(%esi),PARTIAL_REG(%ebx,%bl)
	movb SYM(colors_for_drawing)(,%edx,4),%cl
	movb SYM(colors_for_drawing)(,%ebx,4),%al
	BYTE_MOVE SYM(pixdata)+3(%esi),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE SYM(pixdata)+7(%esi),PARTIAL_REG(%ebx,%bl)
	movb SYM(colors_for_drawing)(,%edx,4),%ch
	addl $4,%esi
	roll $16,%ecx
	movb SYM(colors_for_drawing)(,%ebx,4),%ah
	roll $16,%eax

	movl %ecx,-4(%ebp,%esi)
	cmpl %esi,%edi
	jbe .La_normal_end
	movl %eax,(%ebp,%esi)
	addl $4,%esi
.La_normal_doloop:
	cmpl %esi,%edi
	jnbe .La_normal_loop

.La_normal_end:
        ret

        .globl pfield_linetoscr_hdouble_asm8
	FUNCTYPE (pfield_linetoscr_hdouble_asm8)
	FUNCTION_ALIGN
pfield_linetoscr_hdouble_asm8:
        shrl $1,%esi
	xorl %edx,%edx
	xorl %ebx,%ebx
	movl 36(%esp),%edi /* diw_end */
	shrl $1,%edi
	movl SYM(xlinebuffer),%ebp
	jmp .La_hdouble_doloop
	FUNCTION_ALIGN
.La_hdouble_loop:
        BYTE_MOVE SYM(pixdata)+1(%esi),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE SYM(pixdata)+3(%esi),PARTIAL_REG(%ebx,%bl)
	movl SYM(colors_for_drawing)(,%edx,4),%ecx
	movl SYM(colors_for_drawing)(,%ebx,4),%eax
	BYTE_MOVE SYM(pixdata)+0(%esi),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE SYM(pixdata)+2(%esi),PARTIAL_REG(%ebx,%bl)
	movw SYM(colors_for_drawing)(,%edx,4),%cx
	movw SYM(colors_for_drawing)(,%ebx,4),%ax
	addl $2,%esi

	movl %ecx,-4(%ebp,%esi,2)
	cmpl %esi,%edi
	jbe .La_hdouble_end
 	movl %eax,(%ebp,%esi,2)
	addl $2,%esi
.La_hdouble_doloop:
	cmpl %esi,%edi
	jnbe .La_hdouble_loop

.La_hdouble_end:
        addl %esi,%esi
        ret

        .globl pfield_linetoscr_hdouble_dpf_asm8
	FUNCTYPE (pfield_linetoscr_hdouble_dpf_asm8)
	FUNCTION_ALIGN
pfield_linetoscr_hdouble_dpf_asm8:
        shrl $1,%esi
	xorl %edx,%edx
	xorl %ebx,%ebx
	movl 36(%esp),%edi /* diw_end */
	movl 44(%esp),%ebp
	shrl $1,%edi
	jmp .La_hdouble_dpf_doloop
	FUNCTION_ALIGN
.La_hdouble_dpf_loop:
        BYTE_MOVE SYM(pixdata)+1(%esi),PARTIAL_REG(%edx,%dl)
	CLEAR_FOR_BYTE_MOVE(%ebx)
	BYTE_MOVE SYM(pixdata)+3(%esi),PARTIAL_REG(%ebx,%bl)
	BYTE_MOVE (%ebp,%edx,4),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE (%ebp,%ebx,4),PARTIAL_REG(%ebx,%bl)
	movl SYM(colors_for_drawing)(,%edx,4),%ecx
	movl SYM(colors_for_drawing)(,%ebx,4),%eax
	BYTE_MOVE SYM(pixdata)+0(%esi),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE SYM(pixdata)+2(%esi),PARTIAL_REG(%ebx,%bl)
	BYTE_MOVE (%ebp,%edx,4),PARTIAL_REG(%edx,%dl)
	addl $2,%esi
	BYTE_MOVE (%ebp,%ebx,4),PARTIAL_REG(%ebx,%bl)
	movw SYM(colors_for_drawing)(,%edx,4),%cx
	movw SYM(colors_for_drawing)(,%ebx,4),%ax

	movl SYM(xlinebuffer),%ebx
	movl %ecx,-4(%ebx,%esi,2)
	cmpl %esi,%edi
	jbe .La_hdouble_dpf_end
 	movl %eax,(%ebx,%esi,2)
	addl $2,%esi
.La_hdouble_dpf_doloop:
	cmpl %esi,%edi
	jnbe .La_hdouble_dpf_loop

.La_hdouble_dpf_end:
        addl %esi,%esi
        ret

        .globl pfield_linetoscr_hdouble_ehb_asm8
	FUNCTYPE (pfield_linetoscr_hdouble_ehb_asm8)
	FUNCTION_ALIGN
pfield_linetoscr_hdouble_ehb_asm8:
        shrl $1,%esi
	movl 36(%esp),%edi /* diw_end */
	shrl $1,%edi
	xorl %edx,%edx
	movl SYM(xlinebuffer),%ebp
	jmp .La_hdouble_ehb_doloop
	FUNCTION_ALIGN
.La_hdouble_ehb_loop:
	BYTE_MOVE SYM(pixdata)+1(%esi),PARTIAL_REG(%edx,%dl)
	cmpb $32,%dl
	movl SYM(colors_for_drawing)(,%edx,4),%ecx
	jc   .La_dehb1
	WORD_MOVE SYM(colors_for_drawing)+32*4-32*2(,%edx,2),PARTIAL_REG(%ebx,%bx)
	shrl $1,%ebx
	andl $0x777,%ebx
	movl SYM(xcolors)(,%ebx,4),%ecx
.La_dehb1:

	BYTE_MOVE SYM(pixdata)+3(%esi),PARTIAL_REG(%edx,%dl)
	cmpb $32,%dl
	movl SYM(colors_for_drawing)(,%edx,4),%eax
	jc   .La_dehb2
	WORD_MOVE SYM(colors_for_drawing)+32*4-32*2(,%edx,2),PARTIAL_REG(%ebx,%bx)
	shrl $1,%ebx
	andl $0x777,%ebx
	movl SYM(xcolors)(,%ebx,4),%eax
.La_dehb2:

	BYTE_MOVE SYM(pixdata)+0(%esi),PARTIAL_REG(%edx,%dl)
	cmpb $32,%dl
	movw SYM(colors_for_drawing)(,%edx,4),%cx
	jc   .La_dehb3
	WORD_MOVE SYM(colors_for_drawing)+32*4-32*2(,%edx,2),PARTIAL_REG(%ebx,%bx)
	shrl $1,%ebx
	andl $0x777,%ebx
	movw SYM(xcolors)(,%ebx,4),%cx
.La_dehb3:

	BYTE_MOVE SYM(pixdata)+2(%esi),PARTIAL_REG(%edx,%dl)
	cmpb $32,%dl
	movw SYM(colors_for_drawing)(,%edx,4),%ax
	jc   .La_dehb4
	WORD_MOVE SYM(colors_for_drawing)+32*4-32*2(,%edx,2),PARTIAL_REG(%ebx,%bx)
	shrl $1,%ebx
	andl $0x777,%ebx
	movw SYM(xcolors)(,%ebx,4),%ax
.La_dehb4:
	addl $2,%esi

	movl %ecx,-4(%ebp,%esi,2)
	cmpl %esi,%edi
	jbe .La_hdouble_ehb_end
 	movl %eax,(%ebp,%esi,2)
	addl $2,%esi
.La_hdouble_ehb_doloop:
	cmpl %esi,%edi
	jnbe .La_hdouble_ehb_loop

.La_hdouble_ehb_end:
        addl %esi,%esi
        ret

/* 16 bit functions */

        .globl pfield_linetoscr_asm16
	FUNCTYPE (pfield_linetoscr_asm16)
	FUNCTION_ALIGN
pfield_linetoscr_asm16:
	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 24(%esp),%esi /* start */
	movl 28(%esp),%edi /* lframe_end */
	
	movl SYM(colors_for_drawing),%eax
	
        movl 20(%esp),%ebx
	movl SYM(xlinebuffer),%ecx
	jmp .Lb_do_start_line
	FUNCTION_ALIGN
.Lb_start_line:
	movl %eax,(%ecx,%esi,2)
	movl %eax,4(%ecx,%esi,2)
	addl $4,%esi
.Lb_do_start_line:
	cmpl %esi,%edi
	jnbe .Lb_start_line

	call *%ebx

	/* Finish off the line */

	movl SYM(xlinebuffer),%ecx
        movl 36(%esp),%edi /* stoppos */
	movl SYM(colors_for_drawing),%eax
	jmp .Lb_do_end_line
	FUNCTION_ALIGN
.Lb_end_line:
	movl %eax,(%ecx,%esi,2)
	movl %eax,4(%ecx,%esi,2)
	addl $4,%esi
.Lb_do_end_line:
	cmpl %esi,%edi
	jnbe .Lb_end_line

	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	ret

        .globl pfield_linetoscr_ham6_asm16
	FUNCTYPE (pfield_linetoscr_ham6_asm16)
	FUNCTION_ALIGN
pfield_linetoscr_ham6_asm16:
	movl 36(%esp),%edi /* diw_end */
.Lb_hamloop:
	cmpl %esi,%edi
	jbe .Lb_hamend

	movl SYM(ham_linebuf)+4(,%esi,4),%eax
	movl SYM(xcolors)(,%eax,4),%ebx
	movl SYM(ham_linebuf)(,%esi,4),%eax
	movw SYM(xcolors)(,%eax,4),%bx
	movl %ebx,(%ecx,%esi,2)

	movl SYM(ham_linebuf)+12(,%esi,4),%eax
	movl SYM(xcolors)(,%eax,4),%ebx
	movl SYM(ham_linebuf)+8(,%esi,4),%eax
	movw SYM(xcolors)(,%eax,4),%bx
	movl %ebx,4(%ecx,%esi,2)

	addl $4,%esi
	jmp .Lb_hamloop
.Lb_hamend:
        ret

        .globl pfield_linetoscr_dualpf_asm16
	FUNCTYPE (pfield_linetoscr_dualpf_asm16)
	FUNCTION_ALIGN
pfield_linetoscr_dualpf_asm16:
	movl 44(%esp),%ebp /* lookup_no */
.Lb_dp_loop:
        movl 36(%esp),%eax /* diw_end */
	xorl %edx,%edx
	cmpl %esi,%eax
	jbe .Lb_dp_done

	movl SYM(pixdata)(%esi),%eax
	
	movl SYM(xlinebuffer),%ebx

	movb %ah,%dl
	movb (%ebp,%edx,4),%dl
	movl SYM(colors_for_drawing)(,%edx,4),%ecx
	movb %al,%dl
	movb (%ebp,%edx,4),%dl
	movw SYM(colors_for_drawing)(,%edx,4),%cx
	sarl $16,%eax
	movl %ecx,(%ebx,%esi,2)
	movb %ah,%dl
	movb (%ebp,%edx,4),%dl
	movl SYM(colors_for_drawing)(,%edx,4),%ecx
	movb %al,%dl
	movb (%ebp,%edx,4),%dl
	movw SYM(colors_for_drawing)(,%edx,4),%cx
	movl %ecx,4(%ebx,%esi,2)

	addl $4,%esi
	jmp .Lb_dp_loop

.Lb_dp_done:
	ret

        .globl pfield_linetoscr_ehb_asm16
	FUNCTYPE (pfield_linetoscr_ehb_asm16)
	FUNCTION_ALIGN
pfield_linetoscr_ehb_asm16:
	xorl %edx,%edx
	xorl %ebx,%ebx
.Lb_ehb_loop:
	movl 36(%esp),%edi /* diw_end */
	cmpl %esi,%edi
	jbe .Lb_ehb_end
	
	movl SYM(pixdata)(%esi),%eax
	movb %ah,%bl
	cmpb $32,%ah
	movl SYM(colors_for_drawing)(,%ebx,4),%ecx
	jc   .Lb_ehb2
	WORD_MOVE SYM(colors_for_drawing)+32*4-32*2(,%ebx,2),PARTIAL_REG(%edi,%di)
	shrl $1,%edi
	andl $0x777,%edi
	movl SYM(xcolors)(,%edi,4),%ecx
.Lb_ehb2:
	movb %al,%dl
	movw SYM(colors_for_drawing)(,%edx,4),%cx
	cmpb $32,%al
	jc   .Lb_ehb1

	WORD_MOVE SYM(colors_for_drawing)+32*4-32*2(,%edx,2),PARTIAL_REG(%edi,%di)
	shrl $1,%edi
	andl $0x777,%edi
	movw SYM(xcolors)(,%edi,4),%cx
.Lb_ehb1:
	movl SYM(xlinebuffer),%edi
	sarl $16,%eax
	movl %ecx,(%edi,%esi,2)
	movb %ah,%bl
	movl SYM(colors_for_drawing)(,%ebx,4),%ecx
	cmpb $32,%ah
	jc   .Lb_ehb4

	WORD_MOVE SYM(colors_for_drawing)+32*4-32*2(,%ebx,2),PARTIAL_REG(%edi,%di)
	shrl $1,%edi
	andl $0x777,%edi
	movl SYM(xcolors)(,%edi,4),%ecx
.Lb_ehb4:
	movb %al,%dl
	movw SYM(colors_for_drawing)(,%edx,4),%cx
	cmpb $32,%al
	jc   .Lb_ehb3

	WORD_MOVE SYM(colors_for_drawing)+32*4-32*2(,%edx,2),PARTIAL_REG(%edi,%di)
	shrl $1,%edi
	andl $0x777,%edi
	movw SYM(xcolors)(,%edi,4),%cx
.Lb_ehb3:

	movl SYM(xlinebuffer),%edi
	movl %ecx,4(%edi,%esi,2)
	addl $4,%esi
	jmp .Lb_ehb_loop

.Lb_ehb_end:
        ret

	.globl pfield_linetoscr_normal_asm16
	FUNCTYPE (pfield_linetoscr_normal_asm16)
	FUNCTION_ALIGN
pfield_linetoscr_normal_asm16:
	movl 36(%esp),%edi /* diw_end */
	jmp .Lb_normal_doloop

	FUNCTION_ALIGN
.Lb_normal_loop:
	CLEAR_FOR_BYTE_MOVE(%edx)
	BYTE_MOVE SYM(pixdata)+1(%esi),PARTIAL_REG(%edx,%dl)
	CLEAR_FOR_BYTE_MOVE(%ebx)
	BYTE_MOVE SYM(pixdata)+5(%esi),PARTIAL_REG(%ebx,%bl)

	movl SYM(colors_for_drawing)(,%edx,4),%edi
	movl SYM(colors_for_drawing)(,%ebx,4),%ecx
	BYTE_MOVE SYM(pixdata)+0(%esi),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE SYM(pixdata)+4(%esi),PARTIAL_REG(%ebx,%bl)
	movw SYM(colors_for_drawing)(,%edx,4),%di
	movw SYM(colors_for_drawing)(,%ebx,4),%cx
	BYTE_MOVE SYM(pixdata)+3(%esi),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE SYM(pixdata)+7(%esi),PARTIAL_REG(%ebx,%bl)
	movl SYM(colors_for_drawing)(,%edx,4),%eax
	movl SYM(colors_for_drawing)(,%ebx,4),%ebp
	BYTE_MOVE SYM(pixdata)+2(%esi),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE SYM(pixdata)+6(%esi),PARTIAL_REG(%ebx,%bl)
	movw SYM(colors_for_drawing)(,%edx,4),%ax
	addl $4,%esi
	movw SYM(colors_for_drawing)(,%ebx,4),%bp

	movl SYM(xlinebuffer),%ebx
	movl %edi,-8(%ebx,%esi,2)
	movl 36(%esp),%edi
	movl %eax,-4(%ebx,%esi,2)

	cmpl %esi,%edi
	jbe .Lb_normal_done

	movl %ecx,(%ebx,%esi,2)
	movl %ebp,4(%ebx,%esi,2)

	addl $4,%esi
.Lb_normal_doloop:
	cmpl %esi,%edi
	jnbe .Lb_normal_loop

.Lb_normal_done:
        ret

	.globl pfield_linetoscr_hdouble_asm16
	FUNCTYPE (pfield_linetoscr_hdouble_asm16)
	FUNCTION_ALIGN
pfield_linetoscr_hdouble_asm16:
	shrl $1,%esi
	movl 36(%esp),%eax /* diw_end */
	shrl $1,%eax
	movl SYM(xlinebuffer),%ebp
	CLEAR_FOR_BYTE_MOVE(%edx)
	CLEAR_FOR_BYTE_MOVE(%ebx)
	jmp .Lb_hdouble_doloop

	FUNCTION_ALIGN
.Lb_hdouble_loop:
	BYTE_MOVE SYM(pixdata)(%esi),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE SYM(pixdata)+1(%esi),PARTIAL_REG(%ebx,%bl)

	addl $2,%esi
	movl SYM(colors_for_drawing)(,%edx,4),%edi
	movl SYM(colors_for_drawing)(,%ebx,4),%ecx
	movl %edi,-8(%ebp,%esi,4)
	movl %ecx,-4(%ebp,%esi,4)

.Lb_hdouble_doloop:
	cmpl %esi,%eax
	jnbe .Lb_hdouble_loop

.Lb_hdouble_done:
        addl %esi,%esi
        ret

        .globl pfield_linetoscr_hdouble_dpf_asm16
	FUNCTYPE (pfield_linetoscr_hdouble_dpf_asm16)
	FUNCTION_ALIGN
pfield_linetoscr_hdouble_dpf_asm16:
        shrl $1,%esi
	xorl %edx,%edx
	xorl %ebx,%ebx
	movl 36(%esp),%edi /* diw_end */
	movl 44(%esp),%ebp
	shrl $1,%edi
	cmpl %esi,%edi
	jbe .Lb_hdouble_dpf_end
	jmp .Lb_hdouble_dpf_loop
	FUNCTION_ALIGN
.Lb_hdouble_dpf_loop:
	CLEAR_FOR_BYTE_MOVE(%edx)
        BYTE_MOVE SYM(pixdata)+1(%esi),PARTIAL_REG(%edx,%dl)
	CLEAR_FOR_BYTE_MOVE(%ebx)
	BYTE_MOVE SYM(pixdata)+0(%esi),PARTIAL_REG(%ebx,%bl)
	BYTE_MOVE (%ebp,%edx,4),PARTIAL_REG(%edx,%dl)
	BYTE_MOVE (%ebp,%ebx,4),PARTIAL_REG(%ebx,%bl)
	addl $2,%esi
	movl SYM(colors_for_drawing)(,%edx,4),%ecx
	movl SYM(colors_for_drawing)(,%ebx,4),%eax

	movl SYM(xlinebuffer),%ebx
	movl %eax,-8(%ebx,%esi,4)
	cmpl %esi,%edi
 	movl %ecx,-4(%ebx,%esi,4)
	jnbe .Lb_hdouble_dpf_loop
	jmp  .Lb_hdouble_dpf_end

.Lb_hdouble_dpf_end:
        addl %esi,%esi
        ret

	.globl pfield_linetoscr_hdouble_ehb_asm16
	FUNCTYPE (pfield_linetoscr_hdouble_ehb_asm16)
	FUNCTION_ALIGN
pfield_linetoscr_hdouble_ehb_asm16:
	shrl $1,%esi
	movl 36(%esp),%eax /* diw_end */
	shrl $1,%eax
	movl SYM(xlinebuffer),%ebp
	jmp .Lb_hdouble_ehb_doloop

	FUNCTION_ALIGN
.Lb_hdouble_ehb_loop:
	CLEAR_FOR_BYTE_MOVE(%edx)
	BYTE_MOVE SYM(pixdata)(%esi),PARTIAL_REG(%edx,%dl)
	CLEAR_FOR_BYTE_MOVE(%ebx)
	BYTE_MOVE SYM(pixdata)+1(%esi),PARTIAL_REG(%ebx,%bl)
	addl $2,%esi

	cmpb $32,%dl
	movl SYM(colors_for_drawing)(,%edx,4),%edi
	jc   .Lb_dehb1
	WORD_MOVE SYM(colors_for_drawing)+32*4-32*2(,%edx,2),PARTIAL_REG(%edi,%di)
	shrl $1,%edi
	andl $0x777,%edi
	movl SYM(xcolors)(,%edi,4),%edi
.Lb_dehb1:

	cmpb $32,%bl
	movl SYM(colors_for_drawing)(,%ebx,4),%ecx
	jc   .Lb_dehb2
	WORD_MOVE SYM(colors_for_drawing)+32*4-32*2(,%ebx,2),PARTIAL_REG(%ecx,%cx)
	shrl $1,%ecx
	andl $0x777,%ecx
	movl SYM(xcolors)(,%ecx,4),%ecx
.Lb_dehb2:
	movl %edi,-8(%ebp,%esi,4)
	movl %ecx,-4(%ebp,%esi,4)

.Lb_hdouble_ehb_doloop:
	cmpl %esi,%eax
	jnbe .Lb_hdouble_ehb_loop

.Lb_hdouble_ehb_done:
        addl %esi,%esi
        ret

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
