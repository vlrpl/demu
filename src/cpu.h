#ifndef D_CPU_H
#define D_CPU_H

#include <stdint.h>

#define RAM_SIZE 0x4000


enum emu_arch{
	ARCH_8086 = 0
};

/*
struct cpu_ops {
    //  int (*run)(struct emu_cpu *cpu);
    void (*decode_init)(void);
    void (*translate_init)(void);
    int  (*decode)(void *cpu_arch);
    int  (*translate)(void *cpu_arch);
};
*/

struct emu_cpu {
        void           *cpu_arch;
        uint8_t        *RAM;
        int (*start)(void);
};

register struct emu_cpu *env asm("r14");

//int x86_decode(struct x86_cpu *cpu, struct x86_insn *insn, unsigned char *pc);
//void *x86_translate(struct x86_cpu *cpu, struct x86_insn *insn);

#endif
