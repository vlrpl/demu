#include <string.h>
#include <stdint.h>
#include <udis86.h>
#include "x86_disasm.h"

void llvm_translate_init(void)
{
	//LLVMLinkInJIT();
	//LLVMInitializeNativeTarget();
}

/* just a stub */
int llvm_translate(char *tr_chunk)
{
	return 0;
}


/* just a stub */
void dyn_translate_init(void)
{
	return;
}

static inline int copy_micro_op(char *dest, char *src)
{
	int count = 0;

	while(*(src+4) != 0xc3 && *((uint32_t *)src) != 0x90909090){
		*dest++ = *src++;
		count++;
	}

	return count;
}

/* translated_chunk[50] */
int dyn_translate(char *tr_chunk)
{
	int op_bytes = 0;
	int tr_bytes = 0;

	ops_ptr   = ops_ptr_buf;
	ops_parms = ops_parms_buf;


	for(;;){
		switch(*ops_ptr++){
		case ID_op_movl_T0_im:
		{
			uint32_t param1;
			extern void op_movl_T0_im();
			op_bytes = copy_micro_op(tr_chunk, (char *)&op_movl_T0_im+0);
			param1 = *ops_parms++; 
			*(uint32_t *)(tr_chunk + 2) = param1;
			tr_bytes += op_bytes;
			tr_chunk += op_bytes;
			//dlog("generated code:\n");
			break;
		}
		case ID_op_movl_AX_T0:
		{
			extern void op_movl_AX_T0(void);
			op_bytes = copy_micro_op(tr_chunk, (char *)&op_movl_AX_T0+0);
			tr_bytes += op_bytes;
			tr_chunk += op_bytes;
			//dlog("generated code:\n");
			break;
		}
		case ID_op_movl_BX_T0:
		{
			extern void op_movl_BX_T0(void);
			op_bytes = copy_micro_op(tr_chunk, (char *)&op_movl_BX_T0+0);
			tr_bytes += op_bytes;
			tr_chunk += op_bytes;
			//dlog("generated code:\n");
			break;
		}
			/* case ID_op_movl_CX_T0: */
			/* 	break; */
			
			/* case ID_op_movl_DX_T0: */
			/* 	break; */
			
		case ID_op_movl_T0_AX:
		{
			extern void op_movl_T0_AX(void);
			op_bytes = copy_micro_op(tr_chunk, (char *)&op_movl_T0_AX+0);
			tr_bytes += op_bytes;
			tr_chunk += op_bytes;
			//dlog("generated code:\n");
			break;
		}	

		case ID_op_movl_T0_BX:
		{		
			extern void op_movl_T0_BX(void);
			op_bytes = copy_micro_op(tr_chunk, (char *)&op_movl_T0_BX+0);
			tr_bytes += op_bytes;
			tr_chunk += op_bytes;
			//dlog("generated code:\n");
			break;
		}
			/* case ID_op_movl_T0_CX: */
			/* 	break; */
			
			/* case ID_op_movl_T0_DX: */
			/* 	break; */
		case ID_op_none:
		case ID_op_retq:
		default:
			goto exit_loop;

		}
	}

exit_loop:
	return tr_bytes;
}

/*
 * Local Variables: 
 * c-file-style: "linux"
 * indent-tabs-mode: t
 * c-basic-offset: 8
 * tab-width: 8
 * End:
 */
