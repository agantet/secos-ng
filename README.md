# secos-ng (x86)

secos-ng est la nouvelle version de [SECOS](https://github.com/sduverger/secos), un projet éducatif permettant d'**appliquer pas à pas les concepts d'OS et de sécurité** abordés en cours à l'aide de petits TPs, pour la plateforme x86-32bits.
Le noyau est simpliste. Il est démarré par Grub, en mode protégé en 32 bits, partiellement initialisé par Grub avant son démarrage. **L'objectif est, au fil des TPs, de le configurer petit à petit et de manière sécurisée.**

* Le [guide suivant](#quick-start-guide) indique, en bref, comment installer, trouver les énoncés et démarrer un TP.
* Le [wiki](https://github.com/agantet/secos-ng/wiki), plus complet, détaille notamment l'architecture du projet et son API.

# Quick start guide

## Installation et dépendences

L'environnement de déploiement s'appuyant sur QEMU, il est nécessaire
d'installer les paquets suivants :

```bash
bash$ sudo apt-get install qemu-system-x86 qemu-kvm gcc-multilib make git
bash$ git clone https://github.com/agantet/secos-ng
```

## Organisation du projet

Le code de secos-ng est organisé de la façon suivante :

* [kernel](./kernel) contient l'implémentation de quelques composants de base. Sauf indication contraire, il n'est pas nécessaire de modifier les fichiers de ce répertoire.
* [utils](./utils) contient l'ensemble des configurations nécessaires à la compilation, au lancement et au debug de secos-ng. Sauf indication contraire, il n'est pas nécessaire de modifier les fichiers de ce répertoire.
* Chaque répertoire `tpX` possède le squelette de code et l'environnement nécessaire pour créer un noyau incluant de plus en plus de fonctionnalités bas niveau.

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

QEMU va alors démarrer une VM équipée d'un processeur x86 32 bits et de 128MB de RAM. La VM démarre sur une disquette "grub.floppy" qui contient le bootloader Grub. Celui-ci est configuré pour charger le fichier `kernel.elf` depuis le disque dur "HDD0". Le fichier `kernel.elf` est notre noyau d'OS, il se trouve dans le répertoire de TP courant.
Au lancement, le terminal courant va être utilisé par le mode monitor de QEMU et va afficher des messages de logs (le port série de la VM est redirigé dans le terminal (stdio)).  La VM ne dispose pas d'affichage graphique. 


## Raccourcis QEMU utiles à connaitre

|Raccourci| Utilité|
|-|-|
|**\<ctrl-a\> \<x\>**| Pour quitter (et stopper) la VM |
|**\<ctrl-a\>** puis **\<c\>** puis **\<enter\>**| Pour accéder au "monitor" de QEMU (inspection de debug de la machine) et en ressortir. Note : Le monitor est multiplexé avec le port série sur stdio |
| **q**| Pour quitter le mode monitor de QEMU |

## Contributions élèves

Les TP sont à réaliser dans l'ordre suivant :

* [`tp0`](./tp0) : Démarrage
* [`tp1`](./tp1) : Segmentation
* [`tp2`](./tp2) : Interruptions et exceptions
* [`tp3`](./tp3) : Niveau de privilèges
* [`tp4`](./tp4) : Pagination
* [`tp5`](./tp5) : Appels systèmes
* [`tp_exam`](./tp_exam) : OS complet comportant 2 applications

Dans chacun des répertoires, le fichier `README.md` contient **l'énoncé**, et le
fichier `tp.c` est celui dans lequel **les développements sont attendus**. 

**Note : si une question est marquée d'une `*`, c'est que le code rédigé pour
  y répondre peut à nouveau servir dans la suite des TP, voire pour le TP
  exam.**

# En savoir plus

Cf. [wiki](https://github.com/agantet/secos-ng/wiki)
