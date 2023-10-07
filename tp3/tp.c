/* GPLv2 (c) Airbus */
#include <debug.h>

void userland() {
   asm volatile ("mov %eax, %cr0");
}

void tp() {
   // TODO
}
