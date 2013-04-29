#ifndef X86_DISASM_H
#define X86_DISASM_H

enum{
	ID_op_none = 0,
	ID_op_movl_T0_im,
	ID_op_movl_AX_T0,
	ID_op_movl_BX_T0,
	ID_op_movl_T0_AX,
	ID_op_movl_T0_BX,
	ID_op_retq,
	ID_op_not_supported,
};

#define OP_SRC  1
#define OP_DEST 0

extern uint16_t ops_ptr_buf[50];
extern uint16_t ops_parms_buf[50];

extern uint16_t *ops_ptr;   // = ops_buf;
extern uint16_t *ops_parms; // = ops_parms_buf;

#endif
