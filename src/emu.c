#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <emu.h>
#include "cpu.h"

int __op_param1;
char *me;

void usage(void)
{
	printf("Usage: %s <translator> <flat_binary>\n", me);
	printf("\ttranslator: llvm | dyn\n");
}

static inline void env_start(void)
{
	env->start();
}

int main(int argc, char **argv)
{
	long param1;
	int dyn_tr;
    
	me = argv[0];
    
	if (argc != 3)
		usage();
    
	if (!strncmp(argv[1], "llvm", 4))
		dyn_tr = EMU_LLVM;
	else if (!strncmp(argv[1], "dyn", 3))
		dyn_tr = EMU_DYN;
	else
		dyn_tr = EMU_INTRP; 
    
	if (cpu_init(ARCH_8086, dyn_tr)){
		printf("Panic no CPU\n");
		return -1;
	}

	ram_init(RAM_SIZE);

	if (load_executable(argv[2]) == -1)
		assert(0);

	env_start();
	cpu_remove();
}

/*
 * Local Variables: 
 * c-file-style: "linux"
 * indent-tabs-mode: t
 * c-basic-offset: 8
 * tab-width: 8
 * End:
 */
