#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "cpu.h"

// struct emu_cpu cpu;
//register struct emu_cpu *env asm("r14");

void cpu_remove(void)
{
	free(env);
}

int load_executable(char *filename)
{
	FILE *fd;
	struct stat file_stat;
  
	/* load code */
	if (!(fd = fopen(filename, "rb"))) {
		printf("Could not open %s!\n", filename);
		return -1;
	}
  
	if (fstat(fileno(fd), &file_stat) == -1)
		goto exit_gracefully;

	(void)fread(env->RAM, 1, file_stat.st_size, fd);

	if (ferror(fd))
		goto exit_gracefully;

	fclose(fd);
	return 0;

exit_gracefully:
        fclose(fd);
	return -1;
}

void ram_init(size_t ram_size)
{
	uint8_t *r;

	r = (uint8_t *)malloc(ram_size);
	
	if (r == NULL)
		assert(0);
	
	memset(r, 0xff, ram_size);
	
	env->RAM = r;
}

extern int cpu_x86_new(int dt_be);
int cpu_init(enum emu_arch arch, int dt)
{
	int c;

	env = calloc(1, sizeof(struct emu_cpu));

	switch(arch)
	{

	case ARCH_8086:
		c = cpu_x86_new(dt);
		break;

	default:
		assert(0);
	}
	
	return c;
}

/*
 * Local Variables: 
 * c-file-style: "linux"
 * indent-tabs-mode: t
 * c-basic-offset: 8
 * tab-width: 8
 * End:
 */
