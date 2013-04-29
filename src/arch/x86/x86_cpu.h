#ifndef X86_CPU_H
#define X86_CPU_H

enum{
  AX=0,
  BX, 
  CX,
  DX,
};


struct x86_cpu {
        uint16_t gpr[4];	
	uint16_t pc;
};



#endif
