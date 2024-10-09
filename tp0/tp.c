/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp() {
   debug("kernel mem [0x%p - 0x%p]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);

   multiboot_memory_map_t* entry = (multiboot_memory_map_t*)info->mbi->mmap_addr;
   while((uint32_t)entry < (info->mbi->mmap_addr + info->mbi->mmap_length)) {
      // Question 2
      // TODO print "[start - end] type" for each entry
      char *mbm[] = {"MULTIBOOT_MEMORY_AVAILABLE", 
                     "MULTIBOOT_MEMORY_RESERVED", 
                     "MULTIBOOT_MEMORY_ACPI_RECLAIMABLE", 
                     "MULTIBOOT_MEMORY_NVS"};
      debug("[0x%x - 0x%x] %s\n", (unsigned int)entry->addr, (unsigned int)(entry->addr + entry->len - 1), mbm[entry->type-1]);
      entry++;
   }

   // Question 3
   int *ptr_in_available_mem;
   ptr_in_available_mem = (int*)0x010;
   debug("Available mem (0x0): \nbefore: 0x%x \n", *ptr_in_available_mem);       // read
   *ptr_in_available_mem = 0xaaaaaaaa;                                           // write
   debug("after: 0x%x\n", *ptr_in_available_mem);                                // check

   // Ici la valeur a bien été modifié car c'est une zone mémoire disponible
   // (READ & WRITE)

   int *ptr_in_reserved_mem;
   ptr_in_reserved_mem = (int*)0xf0010;
   debug("Reserved mem (at: 0xf0000):  \nbefore: 0x%x \n", *ptr_in_reserved_mem);    // read
   *ptr_in_reserved_mem = 0xaaaaaaaa;                                                // write
   debug("after: 0x%x\n", *ptr_in_reserved_mem);                                     // check

   // On constate que la valeur stocké à 0xf0010 n'a pas été modifié car elle est RESERVE !!!
   // (READ ONLY)


   // Question 4 :
   // Lecture / Ecriture en dehors de la taille de la mémoire physique :
   // Cas de QEMU : Le contrôleur de ram ne lis que les bits de poid faible (modulo) pour ensuite lire/écrire dessus.
   // Ne lève donc pas de faute
   // Cas Machine physique : ne se passe rien car pas de mapping sur une adresse physique disponible.
}
