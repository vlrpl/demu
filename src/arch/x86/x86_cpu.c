#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <udis86.h>
#include <assert.h>
#include <emu.h>
#include <cpu.h>
#include "x86_cpu.h"
#include "x86_disasm.h"

int env_x86_start(void);

extern void llvm_translate_init(void);
extern int llvm_translate(char *tr_chunk);
extern void dyn_translate_init(void);
extern int dyn_translate(char *tr_chunk);

void (*translate_init)(void);
//int  (*translate)(ud_t *d, struct tb_s *tb);
int  (*translate)(char *tr_chunk);

int cpu_x86_new(int dt_be)
{
	struct x86_cpu *x86_cpu;
  
	x86_cpu = (struct x86_cpu *)malloc(sizeof(struct x86_cpu));
	
	if (x86_cpu == NULL)
		return -1;
	
	if (dt_be == EMU_LLVM){
		translate_init = &llvm_translate_init;
		translate = &llvm_translate;
	}else if (dt_be == EMU_DYN){
		translate_init = &dyn_translate_init;
		translate = &dyn_translate;
	}/*else{ 
		translate_init = &translate_reg_init;
		translate = &reg_translate;
	}
	 */
	env->cpu_arch = x86_cpu;
	env->start    = &env_x86_start; 
  
	return 0;
}

static inline void cpu_x86_remove()
{
	free(env->cpu_arch);
}

static inline void tb_set_x(struct tb_s *tb)
{
	if( mprotect((void *)tb, sizeof(struct tb_s), PROT_EXEC | PROT_WRITE | PROT_READ) != 0 ){
		perror("Not executable");
		assert(0);
	}
}

void tb_set_slide(struct tb_s *tb)
{
	memset(tb, 0x90, sizeof(struct tb_s));
	tb->ret = 0xc3;
}


static inline void ops_buf_init(void)
{
	ops_ptr   = ops_ptr_buf;
	ops_parms = ops_parms_buf;
	
	memset(ops_ptr, 0, sizeof(ops_ptr_buf));
	memset(ops_parms, 0, sizeof(ops_parms));
}

/*
 * returns the number of the host bytes translated
 */
int translate_block(ud_t *insn, struct tb_s *tb_buf, size_t insns_num)
{ 
	int bd, tr_bytes = 0;
	size_t iin = insns_num;
	
	tb_set_slide(tb_buf);

	ops_buf_init();

	while (insns_num){
	
		if ((!(bd = disasm(insn))) 
		    && !(iin - insns_num))
			goto dont_translate;

		if (!bd)
			break;

		insns_num--;
		/* redundant if removed, remove the line in disasm */ 
		((struct x86_cpu *)(env->cpu_arch))->pc += bd; 
	}

	tr_bytes = translate(tb_buf->tb);

dont_translate:
	return tr_bytes; 
}

void dump_cpu(struct x86_cpu *cpu)
{
	printf("Registers dump:\n");
	printf("AX: %d, BX: %d, CX: %d, DX: %d\n", cpu->gpr[AX], cpu->gpr[BX], cpu->gpr[CX], cpu->gpr[DX]);
}

int env_x86_start(void)
{
	struct tb_s tb_buf;
	ud_t d;

	tb_set_x(&tb_buf);
	disasm_init(&d);
	translate_init();

	for(;;){
		if (translate_block(&d, &tb_buf, 1) == 0)
			goto exit_gracefully;

		BUF_EXEC(&tb_buf);
		dump_cpu((struct x86_cpu *)env->cpu_arch);
	}
  
exit_gracefully:
	cpu_x86_remove();
	return 0;
}

/*
 * Local Variables: 
 * c-file-style: "linux"
 * indent-tabs-mode: t
 * c-basic-offset: 8
 * tab-width: 8
 * End:
 */
