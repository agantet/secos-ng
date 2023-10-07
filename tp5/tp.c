/* GPLv2 (c) Airbus */
#include <debug.h>

void syscall_isr() {
   asm volatile (
      "leave ; pusha        \n"
      "mov %%esp, %eax      \n"
      "call syscall_handler \n"
      "popa ; iret"
      );
}

void __regparm__(1) syscall_handler(int_ctx_t *ctx) {
   debug("SYSCALL eax = %p\n", ctx->gpr.eax);
}

void userland() {
    // TODO à compléter
   while(1);
}

void tp() {
    // TODO
}
