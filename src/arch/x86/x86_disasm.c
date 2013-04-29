#include <udis86.h>
#include <assert.h>
#include <cpu.h>
#include "x86_cpu.h"
#include "x86_disasm.h"


uint16_t ops_ptr_buf[50];
uint16_t ops_parms_buf[50];

uint16_t *ops_ptr;   // = ops_buf;
uint16_t *ops_parms; // = ops_parms_buf;

#define EMU_ENOSUP -1
void enqueue_micro_op(ud_t *insn)
{
	switch(insn->mnemonic){
	case UD_Imov:
		/* immediate to register 
		 * register to register
		 */
		if (insn->operand[OP_SRC].type == UD_OP_IMM){
			*ops_ptr++   = ID_op_movl_T0_im;
			*ops_parms++ = insn->operand[OP_SRC].lval.uword;

			if (insn->operand[OP_DEST].base == UD_R_AX){
				*ops_ptr++ = ID_op_movl_AX_T0;
			
			}else if (insn->operand[OP_DEST].base == UD_R_BX){
				*ops_ptr++ = ID_op_movl_BX_T0;
			
			}else{
				printf("Unsupported register\n");
				assert(0);
			}

		}else if (insn->operand[OP_SRC].type == UD_OP_REG){

			if ((insn->operand[OP_SRC].base == UD_R_AX) 
			    && (insn->operand[OP_DEST].base == UD_R_BX)){
				*ops_ptr++ = ID_op_movl_T0_AX;
				*ops_ptr++ = ID_op_movl_BX_T0;
			}else if ((insn->operand[OP_SRC].base == UD_R_BX) 
			    && (insn->operand[OP_DEST].base == UD_R_AX)){
				*ops_ptr++ = ID_op_movl_T0_BX;
				*ops_ptr++ = ID_op_movl_AX_T0;
			}else{
				printf("Unsupported register\n");
				assert(0);
			}
		}

		break;

	case UD_Iret:
		*ops_ptr++ = ID_op_retq;
		break;

	case UD_Iinvalid:
		break;

	case UD_Iadd:
	case UD_Isub:
	default:
		*ops_ptr++ = ID_op_not_supported;
		printf("Unsupported instruction disassembled\n");
		
	
	}
}

/* void decode_init(ud_t *d) */
void disasm_init(ud_t *d)
{
	ud_init(d);
	ud_set_input_buffer(d, env->RAM, RAM_SIZE/*size_t size*/);
	ud_set_mode(d, 16);
	ud_set_syntax(d, &enqueue_micro_op /*UD_SYN_ATT*/);  
#define START_OF_CODE 0
	ud_set_pc(d, START_OF_CODE);
#undef  START_OF_CODE
}

/* int decode(ud_t *d) */
int disasm(ud_t *d)
{
	int retval;

	ud_set_pc(d, (unsigned long long)((struct x86_cpu *)(env->cpu_arch))->pc);
	retval = ud_disassemble(d);

	if (d->mnemonic == UD_Iinvalid)
		retval = 0;

	return retval;
} 

#define MAX_FUNCTION_LENGTH     512
#define X86_64_INSN_RET         0xc3
size_t x86_64_function_size(void *buf) 
{
	unsigned int len = 0, dbytes;
	struct ud disas1, *disas=&disas1;

	ud_init(disas);
	ud_set_mode(disas, 64);
	ud_set_input_buffer(disas, buf, (size_t)MAX_FUNCTION_LENGTH);

	while (dbytes = ud_disassemble(disas)) {
		len += dbytes; //ud_insn_len(disas);
		if ((/* ud_insn_len(disas) */dbytes == 1) 
		    && (*ud_insn_ptr(disas) == X86_64_INSN_RET)) 
			return len - 1;
		
	}

	return -1;
}
			
void x86_64_block_disas(void *buf, size_t n) 
{
	// Disassemble the buffer for interest 
	unsigned int len = 0;
	struct ud disas1, *disas=&disas1;

	ud_init(disas);
	ud_set_mode(disas, 64);
	ud_set_syntax(disas, UD_SYN_ATT);
	ud_set_input_buffer(disas, buf, (size_t)MAX_FUNCTION_LENGTH);

	while (len < n) {
		len += ud_disassemble(disas);
		printf("%08lx %-25s %s\n", (uint64_t)(buf)+ud_insn_off(disas),
		       ud_insn_hex(disas), ud_insn_asm(disas));
		if ((ud_insn_len(disas) == 1)
		    && (*ud_insn_ptr(disas) == X86_64_INSN_RET)) 
			break;
		
	}
	return;
}
#undef MAX_FUNCTION_LENGTH
#undef X86_64_INSN_RET

		   

/*
 * Local Variables: 
 * c-file-style: "linux"
 * indent-tabs-mode: t
 * c-basic-offset: 8
 * tab-width: 8
 * End:
 */
