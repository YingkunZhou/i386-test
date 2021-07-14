#include "insn.h"
char *str = "hello\n";

static inline void print(void)
{
  __asm__ __volatile__(
      "movl $6, %%edx \n\t"
      "movl %0, %%ecx \n\t"
      "movl $1, %%ebx \n\t"
      "movl $4, %%eax \n\t"
      "int $0x80 \n\t"
      ::"r"(str): "edx", "ecx", "ebx");
}

static inline void finish(void)
{
  __asm__ __volatile__(
      "movl $42, %ebx \n\t"
      "movl $1, %eax \n\t"
      "int $0x80 \n\t");
}

void test()
{
  print();
  AAA();
  AAD();
  AAM();
  ADDPD();
  ADDPS();
  ADDSD();
  ADDSS();
  ADDSUBPD();
  ARPL();
  BLENDVPD();
  finish();
}
