#include <cpu.h>
#include "x86_cpu.h"

#define FORCE_RET() asm volatile("nop;nop;nop;nop;")

register unsigned long T0 asm("r15");

#define R_AX ((struct x86_cpu *)env->cpu_arch)->gpr[AX]
#define R_BX ((struct x86_cpu *)env->cpu_arch)->gpr[BX]
#define R_CX ((struct x86_cpu *)env->cpu_arch)->gpr[CX]
#define R_DX ((struct x86_cpu *)env->cpu_arch)->gpr[DX]

#define DECLARE_ADD_REG_T0(reg)			\
	void op_addl_ ## reg ## _T0(void)	\
	{					\
		R_ ## reg += T0;		\
		FORCE_RET();		        \
	}

#define DECLARE_MOV_REG_T0(reg)			\
	void op_movl_T0_ ## reg(void)		\
	{					\
		T0 = R_ ## reg;			\
		FORCE_RET(); 	 	        \
	}					
	
#define DECLARE_MOV_T0_REG(reg)			\
	void op_movl_ ## reg ## _T0(void)	\
	{					\
		R_ ## reg = T0;			\
		FORCE_RET();		        \
	}

extern int __op_param1;
void op_movl_T0_im(void)
{
	T0 = ((long)(&__op_param1));
	FORCE_RET();
}

DECLARE_ADD_REG_T0(AX)
DECLARE_ADD_REG_T0(BX)
DECLARE_ADD_REG_T0(CX)
DECLARE_ADD_REG_T0(DX)

DECLARE_MOV_T0_REG(AX)
DECLARE_MOV_T0_REG(BX)
DECLARE_MOV_T0_REG(CX)
DECLARE_MOV_T0_REG(DX)

DECLARE_MOV_REG_T0(AX)
DECLARE_MOV_REG_T0(BX)
DECLARE_MOV_REG_T0(DX)
DECLARE_MOV_REG_T0(CX)


/*
 * Local Variables: 
 * c-file-style: "linux"
 * indent-tabs-mode: t
 * c-basic-offset: 8
 * tab-width: 8
 * End:
 */
