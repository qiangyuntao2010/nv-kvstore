#include <stdio.h>

void print_binary(int number)
{
    if (number) {
        print_binary(number >> 1);
        putc((number & 1) ? '1' : '0', stdout);
    }
}

static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
                                unsigned int *ecx, unsigned int *edx)
{
        /* ecx is often an input as well as an output. */
        asm volatile("cpuid"
            : "=a" (*eax),
              "=b" (*ebx),
              "=c" (*ecx),
              "=d" (*edx)
            : "0" (*eax), "2" (*ecx));
}

int main(int argc, char **argv)
{
  unsigned eax, ebx, ecx, edx;

  /* processor info and feature bits */
  eax = 1;
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("clfsh support: %d\n", ((edx >> 19) & 0x1));

  ///* extended feature */
  eax = 7;
  ecx = 0;
  native_cpuid(&eax, &ebx, &ecx, &edx);
  printf("Cache line related flag: %03d\nExtended feature flag: ", (ebx >> 22) & 0x7);
  print_binary(ebx); putc('\n',stdout);
}
