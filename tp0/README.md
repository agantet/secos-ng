# TP0 : Familiarisation avec le noyau SECOS

Le but du TP0 est de se familiariser avec le noyau SECOS.

## Comprendre la localisation du point d'entrée

Le point d'entrée d'un programme se définit par l'adresse à laquelle la
première instruction va être lue et exécutée. Dans le cas de `kernel.elf`, ce
programme est linké grâce au LD-script [linker.lds](../utils/linker.lds) qui
définit l'agencement mémoire du noyau lorsqu'il va être chargé par le
bootloader GRUB. Notamment, ce linker spécifie en dur à quelle adresse l'ELF
`kernel.elf` doit être chargé (`0x300000`) :

```c
SECTIONS
{
   . = 0x300000;
```

Cependant,  `readelf` informe que le point d'entrée est à `0x302010` :

```bash
(tp0)$ readelf -l kernel.elf

Elf file type is EXEC (Executable file)
Entry point 0x302010
There are 3 program headers, starting at offset 52

Program Headers:
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  LOAD           0x000094 0x00300000 0x00300000 0x0000c 0x0000c RWE 0x4
  LOAD           0x0000a0 0x00300010 0x00300010 0x00000 0x02000 RW  0x10
  LOAD           0x0000b0 0x00302010 0x00302010 0x013f8 0x01810 RWE 0x20
```

Par ailleurs, lors du démarrage, le noyau TP0 affiche sur le port série le
message suivant concernant la zone mémoire qu'il occupe (affichage entre
`__kernel_start__` et `__kernel_end__`, cf. [tp.c](./tp.c#l10)) :

```bash
(tp0)$ make qemu
secos-a241db6-59e4545 (c) Airbus
kernel mem [0x302010 - 0x303820]
```

**Q1 : A l'aide du [linker.lds](../utils/linker.lds) et de [entry.s](../kernel/core/entry.s), 
déterminer la taille de la section `.mbh`  et `.stack` et en déduire d'où
provient la valeur de point d'entrée à `0x302010` au lieu de `0x300000`.**

Note : il est possible de s'aider également de la page wiki sur les options de [linkage](https://github.com/agantet/secos-ng/wiki/Tooling#options-de-linkage).


## Cartographie mémoire au démarrage

Au démarrage d'un noyau de système, il peut être intéressant de prendre
connaissance de la cartographie mémoire en place à ce moment-là, par exemple
pour obtenir des informations sur la RAM dont il va pouvoir disposer.

Avant que le noyau ait la main, le bootloader GRUB s'est exécuté et a
notamment commencé à remplir de nombreuses informations système dans un objet
conforme au format [Multiboot Information version 1](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html) (MBI). Des définitions de structures liées au MBI sont fournies dans SECOS : 
[`include/mbi.h`](../kernel/include/mbi.h) et [`include/grub_mbi.h`](../kernel/include/grub_mbi.h). Par défaut
dans SECOS, ces informations peuvent être accédées dans le champ `mbi` 
de l'objet global pré-initialisé [`info`](./tp.c#l5) de type [`info_t`](../kernel/include/info.h).

Parmi les informations MBI, le champ `mmap_addr` contient l'adresse du premier
objet de type [`multiboot_memory_map`](../kernel/include/grub_mbi.h/#l243) 
(début de la liste) permettant d'obtenir des informations sur la mémoire. Le
champ `mmap_length` contient la taille de liste d'objets `multiboot_memory_map`.

**Q2 : Compléter la fonction `tp()` de [tp.c](./tp.c) pour naviguer dans la
structure `info` de telle sorte à pouvoir construire une cartographie mémoire
comme celle ci-dessous et l'afficher dans les logs de debug de SECOS :**

```
[0x0 - 0x9fbff] MULTIBOOT_MEMORY_AVAILABLE
[0x9fc00 - 0x9ffff] MULTIBOOT_MEMORY_RESERVED
[0xf0000 - 0xfffff] MULTIBOOT_MEMORY_RESERVED
[0x100000 - 0x7fdffff] MULTIBOOT_MEMORY_AVAILABLE
[0x7fe0000 - 0x7ffffff] MULTIBOOT_MEMORY_RESERVED
[0xfffc0000 - 0xffffffff] MULTIBOOT_MEMORY_RESERVED
```

## Utilisation d'adresses mémoire sans configuration supplémentaire

**Q3 : Compléter la fonction `tp()` de [tp.c](./tp.c) pour tester le code
  suivant et observer ce qu'il se passe en lisant/écrivant dans une zone de
  mémoire libre, et en lisant/écrivant dans une zone de mémoire réservée, par
  exemple ainsi :**

```c
int *ptr_in_available_mem;
ptr_in_available_mem = (int*)0x0;
debug("Available mem (0x0): before: 0x%x ", *ptr_in_available_mem); // read
*ptr_in_available_mem = 0xaaaaaaaa;                           // write
debug("after: 0x%x\n", *ptr_in_available_mem);                // check

int *ptr_in_reserved_mem;
ptr_in_reserved_mem = (int*)0xf0000;
debug("Reserved mem (at: 0xf0000):  before: 0x%x ", *ptr_in_reserved_mem); // read
*ptr_in_reserved_mem = 0xaaaaaaaa;                           // write
debug("after: 0x%x\n", *ptr_in_reserved_mem);                // check
```
Le comportement observé semble-t-il cohérent ?

**Q4 : Compléter la fonction `tp()` de [tp.c](./tp.c) pour essayer de lire ou
  écrire à une adresse en dehors de la mémoire physique disponible (128 MB).
  Que se passe-t-il ? Comment pourrait-on l'expliquer ?**
