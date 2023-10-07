# TP1 - La segmentation

Le but du TP est de bien comprendre la notion de segmentation.

Le fichier [`kernel/include/segmem.h`](../kernel/include/segmem.h). contient
les déclarations de nombreuses informations, de structures et de macros utiles
pour la résolution du TP.

**Note : QEMU n'émule pas "complètement" la segmentation.**  Pour ce TP, il
  est nécessaire d'utiliser KVM à la place. Pour cela, il suffit de modifier
  le fichier [`utils/config.mk`](../utils/config.mk) et remplacer ` QEMU := $
  (shell which qemu-system-x86_64)` par `QEMU := $(shell which kvm)`.

## Prérequis théoriques

L'implémentation de configuration de la segmentation dans SECOS étant
fortement liée au matériel x86, il est conseillé d'avoir un manuel Intel à
proximité pour la résolution de ce TP. Pour ce TP, il est notamment utile de
connaître :

* Le registre GDTR, son rôle, son format ;
* Les descripteurs de segments, leur rôle, leur format ;
* Les types de segments possibles (code, data, readable, writable, executable,
  etc.) ; 
* Les sélecteurs de segment, leur rôle, leur format ;
* Les instructions relatives à la configuration de la segmentation telles que
  `SGDT`, `LGDT`.

## Configuration par défaut

Au démarrage, le bootloader GRUB a démarré le noyau en mode protégé. Entre
autres, il a donc dû créer une GDT (Global Descriptor Table) et configurer
les registres liés à la segmentation en conséquence, avant d'exécuter le
point d'entrée du noyau.

**Q1 : Rappeler comment fonctionne l'instruction `SGDT` (rôle, paramètres,
  etc.), puis trouver dans [`kernel/include/segmem.h`](../kernel/include/segmem.h) 
  la macro qui l'utilise pour afficher l'adresse
  de base de la GDT en cours d'utilisation ainsi que sa "limite" (type utile :
  `gdt_reg_t`).**

**Q2\* :  Dans [`tp.c`](./tp.c), un exemple d'implémentation d'affichage du
  contenu de table de type GDT est fournie (fonction `print_gdt_content`).
  L'utiliser pour afficher le contenu de la GDT courante.**

**Q3 : Lire les valeurs des sélecteurs de segment à l'aide des macros prévues
  à cet effet dans [`kernel/include/segmem.h`](../kernel/include/segmem.h), et en déduire quels descripteurs de cette GDT sont en
  cours d'utilisation pour :**

* Le segment de code (sélecteur cs)
* Le segment de données (sélecteur ds)
* Le segment de pile (sélecteur ss)
* D'autres segments (sélecteurs autres : es, fs, gs, etc.)

**Q4 : Que constate-t-on ? Que dire de la ségrégation mémoire mise en place
  par défaut par GRUB avec une telle configuration ?**


## Une première reconfiguration de la GDT : en mode "flat"

Le but est maintenant de configurer une GDT avec une adresse de base, une
taille et un contenu qu'on maîtrise, dans un premier lieu en mode "flat".

### Définition de GDT et descripteurs 

**Q5\* : Choisir une adresse de base pour stocker une nouvelle GDT, et définir les descripteurs ring 0 suivants :**

* Code, 32 bits RX, flat, indice 1
* Données, 32 bits RW, flat, indice 2.

Attention à bien respecter les restrictions matérielles attendues, à savoir :

* L'adresse de base de la GDT doit être alignée sur 8 octets
* Le premier descripteur (indice 0) doit être NULL.

### Utilisation de la nouvelle GDT

Une fois définie, la nouvelle GDT est stockée en mémoire mais n'est pas
encore "utilisée". En effet, pour que le matériel sache quelle table et quels
descripteurs utiliser, il est nécessaire de mettre à jour les registres
système relatifs à la segmentation : GDTR, cs/ss/ds/etc.

**Q6\* : Charger l'adresse de base de la nouvelle GDT dans le registre GDTR,
  ainsi que sa limite, puis mettre à jour les sélecteurs de segment
  (cs/ss/ds/...) afin qu'ils pointent vers les descripteurs précédemment
  définis.**

**Q7 : Rappeler `print_gdt_content()` pour s'assurer que la nouvelle GDT est
  bien utilisée.**

**Q8 : Essayer de charger un descripteur de segment de code dans le sélecteur
  DS. Que se passe-t-il ? Est-ce conforme avec ce que décrit la documentation
  Intel à ce sujet ? Faire de même avec un descripteur de segment de données
  pour le sélecteur CS.**


## Exemple d'utilisation de la segmentation pour la sécurité

Le code suivant initialise un buffer `src` qu'il remplit de 64 octets `0xff`
ainsi que le pointeur `dst`, puis copie dans `dst` le contenu de ce qu'il y a
dans `src` :

```c
  #include <string.h>

  char  src[64];
  char *dst = 0;

  memset(src, 0xff, 64);
  _memcpy8(dst, src, 32);
```

Note : L'implémentation de `_memcpy8()` de SECOS repose sur l'instruction x86
`REP MOVSB` qui utilise le registre `es`.

**Q9 : Dans la GDT précédente, définir une nouvelle entrée contenant un descripteur ayant les caractéristiques suivantes :**

 - **data, ring 0**
 - **32 bits RW**
 - **base 0x600000**
 - **limite 32 octets**

**Q10 : Charger le sélecteur de segment "es" de manière à adresser ce nouveau
  descripteur de données puis ré-exécuter la copie ` _memcpy8(dst, src,
  32);`. Que se passe-t-il ? Pourquoi n'y a-t-il pas de faute mémoire alors
  que le pointeur `dst` est NULL ?**

**Q11 : De même, effectuer à présent une copie de 64 octets. Que se passe-t-il ? Pourquoi ?**

## Première tentative de démarrage de code en ring 3

L'idée est ici d'appréhenter quels aspects de la difficulté d'implémentation
des transitions user (ring3) et kernel (ring0) en x86.

Parmi les attributs des registres système liés à la segmentation, on retrouve
des champs relatifs aux niveaux de privilèges. Le but de cette section est de
tenter de démarrer du code en ring 3. Pour tester si le passage au ring 3
s'est bien passé, une fonction témoin appelée `userland` est fournie dans
[`tp.c`](./tp.c) : cette fonction essaie d'exécuter une action privilégiée,
par exemple mettre à jour le registre de contrôle CR0 :

* si l'exécution de cette fonction `userland` **réussit** (aucune exception levée), c'est que le CPL est toujours à 0 et que le passage au ring 3 a donc **échoué** 
* si l'exécution de cette fonction `userland` **échoue** (exception #GP levée), c'est que le CPL n'est plus à 0 et que le passage au ring 3 a donc **réussi**.

**Q12 : Ajouter à la GDT précédente deux nouveaux descripteurs aux index de votre choix, avec les propriétés suivantes :**

* **Code, 32 bits RX, ring 3, flat**
* **Data, 32 bits RW, ring 3, flat**

**Q13 : Charger progressivement les registres de segments avec des sélecteurs
  qui pointent vers les descripteurs ring 3 :**

* **Que se passe-t-il lors du chargement de DS/ES/FS/GS ?**
* **Que se passe-t-il lors du chargement de SS ?**
* **Concernant la mise à jour du sélecteur CS, effectuer un "far jump" (cf. fonction `farjump()` définie dans [kernel/include/segmem.h](../kernel/include/segmem.h)) vers la fonction `userland()`, avec en paramètre (cf. type `fptr32_t` défini dans [kernel/include/types.h](../kernel/include/types.h)) avec l'adresse de la fonction `userland()` comme nouvel offset, et l'index permettant de sélectionner le descripteur de code ring 3 défini en question Q12 et un RPL à 3 comme nouveau sélecteur.**

**Que se passe-t-il ? Comment un noyau pourrait-il faire autrement pour démarrer une tâche en ring 3 ?**

Cette méthode sera abordée et à implémenter en fin de tp suivant.

## QEMU again

Reconfigurer [`utils/config.mk`](../utils/config.mk) pour ré-utiliser QEMU (et non KVM) pour le TP suivant.