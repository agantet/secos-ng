# SECOS

## Introduction

secos-ng est la nouvelle version de [SECOS](https://github.com/sduverger/secos), 
un projet éducatif permettant d'appliquer pas à pas les concepts d'OS abordés 
en cours à l'aide de petits TPs.

## Pré-requis

Le squelette de code de SECOS est écrit en C, assembleur x86, et comporte
quelques utilisations d'assembleur "inline". Il est donc **recommandé de
savoir lire et programmer en C et assembleur x86.** Compiler un programme
avec GCC et savoir configurer des Makefiles également.

Pour plus de documentation sur l'assembleur en ligne GCC :

 - https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html
 - https://locklessinc.com/articles/gcc_asm/
 - https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html

Il peut également être utile de savoir utiliser les outils suivants :
`objdump`, `readelf`.

## Installation et dépendences

L'environnement de déploiement s'appuyant sur QEMU, il est nécessaire
d'installer les paquets suivants :

```bash
bash$ sudo apt-get install qemu-system-x86 qemu-kvm gcc-multilib make git
bash$ git clone https://github.com/agantet/secos-ng
```

## Arborescence

Le code de SECOS est organisé de la façon suivante :

```bash
$ cd secos-ng ; ls
kernel  README  tp0  tp1  tp2  tp3  tp4  tp5  tp_exam  utils
```

`kernel` contient l'implémentation de quelques composants de base tels que : 

* `printf` / `debug` pour pouvoir afficher des messages sur le port série
* `memcpy`, `memset`, ...
* L'accès aux structures multiboot de Grub
* Les structures matérielles du mode protégé pour x86 : GDT/IDT/LDT,
  descripteurs, sélecteurs, tables de pages, PTE/PDE, TSS, registres de
  contrôle (CR), registres de flags, etc.

Note : `kernel/include` contient notamment des définitions de structures
utilisant des bitfields permettant d'accéder facilement aux champs des
structures matérielles sans avoir à faire de masques ou décallages, utiles
dès le `tp0`.

 Le noyau est simpliste. Il démarre en mode protégé en 32 bits grâce à Grub. A
 ce titre, il accède à la mémoire physique directement sans pagination. Le
 mode protégé a été partiellement initialisé par Grub, mais il faudra, au fil
 des TPs, le configurer petit à petit. Chaque répertoire `tpX` possède le
 squelette de code et l'environnement nécessaire pour créer un noyau incluant
 de plus en plus de fonctionnalités bas niveau :

* [`tp0`](./tp0/README.md) : Démarrage
* [`tp1`](./tp1/README.md) : Segmentation
* [`tp2`](./tp2/README.md) : Interruptions et exceptions
* [`tp3`](./tp3/README.md) : Niveau de privilèges
* [`tp4`](./tp4/README.md) : Pagination
* [`tp5`](./tp5/README.md) : Appels systèmes
* [`tp_exam`](./tp_exam/README.md) : OS complet comportant 2 applications

Dans chacun des répertoires, le fichier `README.md` contient l'énoncé, et le
fichier `tp.c` est celui dans lequel les développements sont attendus. Note :
Il n'est pas nécessaire de modifier l'environnement de compilation ni les
autres fichiers du noyau.

**Note : si une question est marquée d'une `*`, c'est que le code rédigé pour
  y répondre peut à nouveau servir dans la suite des TP, voire pour le TP
  exam.**

## Démarrage d'un TP

Pour démarrer le noyau d'un TP en particulier, il suffit de recompiler le
noyau (`make`), puis de le lancer avec `make qemu`. Par exemple pour le `tp0` :

```bash
$ cd tp0
$ make clean all
    AS    ../kernel/core/entry.s
    CC    ../kernel/core/start.c
    CC    ../kernel/core/print.c
    CC    ../kernel/core/uart.c
    CC    ../kernel/core/pic.c
    CC    tp.c
    LD    kernel.elf

$ make qemu
hit <ctl+a> <h> <enter>, for Qemu help
vvfat . chs 1024,16,63

secos-a241db6-59e4545 (c) Airbus
kernel mem [0x302010 - 0x303820]
```

QEMU va alors démarrer une VM équipée d'un processeur x86 32 bits et de 128MB
de RAM. Au lancement, le terminal courant va être utilisé par le mode monitor
de QEMU et va afficher des messages de logs. Pour quitter (et stopper la VM),
utiliser la séquence **\<ctrl-a\> \<x\>**.

Note : La VM démarre sur une disquette "grub.floppy" qui contient le
bootloader grub. Celui-ci est configuré pour charger le fichier `kernel.elf`
depuis le disque dur "HDD0". Le fichier `kernel.elf` est notre noyau d'OS, il
se trouve dans le répertoire de TP courant.



## Utilisation de l'environnement QEMU

Le monitor de QEMU est utile car permet d'inspecter et debugger la machine
virtuelle.

Pour accéder au "monitor" de QEMU, utiliser la séquence **\<ctrl-a\>**
puis **\<c\>** puis **\<enter\>**. Cela va afficher :

```bash
QEMU 2.xx.yy monitor - type 'help' for more information
(qemu)
```

Par défaut, le port série de la VM est redirigé dans votre terminal(stdio). La
VM ne dispose pas d'affichage graphique. Pour rebasculer dans l'affichage des
logs envoyées sur le port série, tapez de nouveau la
séquence **\<ctrl-a\> \<c\> \<enter\>**. Le monitor est multiplexé avec le
port série sur stdio.

Pour quitter QEMU en mode monitor : **q**.

Note : Le lancement de SECOS sous QEMU est configurable via le fichier
[`config.mk`](./utils/config.mk) qui définit les options à passer à QEMU au
démarrage.

## Moyen de debug

Lire [`README.DEBUG.md`](./README.DEBUG.md) qui contient une aide au debugging offert par l'environnement QEMU.