# TP2 - Les interruptions et les exceptions

Le but du TP est de bien comprendre les interruptions et les exceptions et
comment implémenter des gestionnaires.

## Rappels et prérequis théoriques

Niveau matériel, on a :

* Des informations stockées sur la pile lors de l’arrivée d’une interruption :
    * EFLAGS, CS, EIP, Error Code, seulement si pas de changement de niveau de
      privilèges
    * ESP et SS en supplément en cas de changement de niveau
* Le registre CPU IDTR à renseigner contenant une adresse à laquelle il
  s'attend trouver une table
* Une table de descripteurs d’interruption (IDT), pointeurs vers des routines
  à exécuter.

De son côté, l’OS doit au préalable :

* avoir implémenté des routines de traitement d’interruption
* les avoir référencées correctement dans une zone mémoire contenant une table
  de descripteurs d’interruption (IDT)
* avoir configuré le registre IDTR avec l'adresse de cette zone mémoire.

Dans ce TP, il pourra être utile de se référer à la documentation Intel pour
comprendre en détail ce que font les instructions suivantes et quel est leur
rôle : INT3, RET, IRET. Il pourra être également utile de savoir à quoi
correspond le numéro d'exception #6, ou savoir se servir des instructions
PUSH/PUSHA, POP/POPA.

## Un premier squelette

Notre noyau dispose déjà d'une petite IDT. Elle est configurée dans
[intr.c](../kernel/core/intr.c) par la fonction `intr_init()`,  qui 
a notamment permis d'intercepter les #GP du TP1.

L'IDT contient des descripteurs d'interruptions `int_desc_t` qui sont
initialisés avec leur propre handler d'interruption `isr`.

La valeur d'isr correspond à des petites fonctions que l'on peut voir comme
des trampolines, localisées dans [`idt.s`](../kernel/core/idt.s). Ils servent
à empiler le numéro de l'interruption et à aligner la pile lorsqu'il manque
un code d'erreur pour l'évènement survenu. Chaque trampoline, saute dans
`idt_common` qui appelle le gestionnaire d'interruptions de haut niveau de
notre noyau `intr_hdlr`.

## Notes sur le tracing

Ce TP est également l'occasion d'utiliser les options de "trace" de Qemu,
permettant de savoir ce qu'il se passe dans le CPU durant l'exécution de la
VM.

Dans le fichier [`config.mk`](../utils/config.mk), il est possible
d'ajouter `$(QDBG)` sur la ligne de définition de QOPT, afin de faire prendre
en compte les options de trace au lancement de Qemu. Par défaut, seules
quelques traces sont activées, elles permettent de voir les exceptions
générées.

## Prise en main de l'IDT

**Q1\* : Dans tp.c, localiser l'IDT et afficher son adresse de chargement**
  (cf. fonction `get_idtr()` définie dans [`intr.h`](../kernel/include/intr.h)).

## Gestion furtive des breakpoints #BP

L'idée à présent est de compléter le contenu de l'IDT actuelle, notamment pour
qu'elle puisse gérer l'exception #BP. Le but est de ne pas modifier
`intr_hdlr` mais d'intercepter les #BP en amont depuis [`tp.c`](./tp.c).

### Premier essai naïf : sous forme d'une simple fonction C

**Q2 : Dans [`tp.c`](./tp.c), commencer par écrire une routine de traitement
  dans une fonction, `bp_handler`, affichant un message de debug à
  l'écran.**

**Q3 : Modifier le descripteur d'interruption (cf. type `int_desc_t` défini 
  dans [`intr.h`](../kernel/include/intr.h)) de #BP, stocké
  dans l'IDT, afin d'y référencer `bp_handler()` à la place du trampoline
  déjà installé.**

**Q4 : Pour tester cette mise à jour, ajouter une fonction `bp_trigger`, dans
  [`tp.c`](./tp.c), déclenchant un breakpoint grâce à l'instruction `int3` et
  appeler `bp_trigger()` dans `tp()`.**

**Q5 : Cette implémentation pousse le CPU à générer une faute. Pour comprendre
  pourquoi, à l'aide d'un outil de désassemblage comme `objdump -D`, lister
  les instructions, générées à la compilation, de la fonction `bp_handler()`. 
  Quelle est la dernière instruction de cette fonction ? Quel est son
  impact sur la pile ? Est-ce cohérent avec ce qui était sur la pile au
  moment de l'arrivée d'une interruption ?**

### Deuxième essai : via l'assembleur inline

L'idée est de réécrire `bp_handler` en assembleur inline pour éviter l'écueil
de l'essai précédent.

**Q7 : **Au début de `bp_handler`, afficher la valeur stockée en `ebp-4` :**

```c
  uint32_t val;
   asm volatile ("mov 4(%%ebp), %0":"=r"(val));
```

**Quelle signification cette valeur a-t-elle ? S'aider à nouveau de `objdump -D`
pour comparer cette valeur à une adresse de votre noyau.**

**Q8\* : Qu'est-ce qui n'est pas stocké par le CPU à l'arrivée d'une
  interruption et qu'il est impératif de sauvegarder avant tout traitement de
  l'interruption ? L'implémenter en assembleur inline dans  `bp_handler`.**

**Q9\* : Par quelle instruction doit se terminer la routine pour que le noyau
  rende la main à la fonction tp() ? L'implémenter en assembleur inline dans
  `bp_handler`.**

**Q10 : Tester que le retour du traitement de l'interruption s'est effectué
  correctement en affichant un message de debug dans la fonction `bp_trigger()` 
  après le déclenchement du breakpoint.**

**Q11 : Quelles conclusions peut-on tirer du développement en C d'un
  gestionnaire d'interruption ? Pourquoi l'assembleur semble-t-il plus
  approprié ?**
