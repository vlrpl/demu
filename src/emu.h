#ifndef EMU_H
#define EMU_H

enum {
        EMU_LLVM = 1,
        EMU_DYN,
        EMU_INTRP,
};

#define TB_MAX_SIZE 50
#define BUF_EXEC(b) ({ void (*p)() = (void (*))b; p(); })  //do { __asm__ __volatile__("jmp *%0" ::"r"(b)); }while(0)

struct tb_s{
  uint8_t tb[TB_MAX_SIZE];
  uint8_t ret;
}__attribute__((aligned(0x1000), packed));


#endif
