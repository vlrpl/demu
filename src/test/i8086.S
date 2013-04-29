.code16gcc

.text

.globl  _start

.type   _start, @function

_start:
  movw $1, %ax
  movw $2, %bx

  movw %ax, %bx
#  addw $0x02, %ax 
#  xor %ax, %ax
retw
