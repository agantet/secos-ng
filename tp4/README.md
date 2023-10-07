# TP4 - La pagination

Le but du TP est de bien comprendre la pagination.

Les fichiers [`cr.h`](../kernel/include/cr.h) et [`pagemem.h`](../kernel/include/pagemem.h) 
seront utiles (informations, de structures et de macros, etc.) pour la
résolution du TP.

## Prérequis théoriques

Concernant la pagination :

* Le bit 31 du registre CR0 permet d'activer ou désactiver la pagination (cf. macro `CR0_PG`).
* Le registre CR3 sert à stocker l'adresse du Page Directory (PGD).

## Notes sur les tableaux et pointeurs en C

```c
void fonction()
{
  int *tab_c = (int*)0x1234;
}
```
Le compilateur ne sait pas la taille de la zone mémoire adressée et
potentiellement vous pouvez accéder à toute la mémoire à partir de cette
adresse. Dans une application classique, déclarer un tel pointeur
provoquerait à coup sur un crash de l'application car l'adresse `0x1234`
n'est jamais disponible pour une application (ex. sous Linux). 

Dans notre noyau cela ne pause pas de problème, nous utilisons la
mémoire "physique" pour l'instant et nous n'avons pas de notion de tâche avec
des espaces d'adressage.

## Mise en place de schéma de pagination en identity mapping

**Q1\* : A l'aide de la fonction `get_cr3()`, afficher la valeur courante du
  registre CR3 dans `tp.c`.**

**Q2\* : Allouer un PGD de type `(pde32_t*)` à l'adresse physique `0x600000` et
  mettre à jour `CR3` avec cette adresse.**

**Q3\* : Modifier le registre CR0 de sorte à activer la pagination dans `tp.c`.
  Que se passe-t-il ? Pourquoi ?**

**Q4\* : Un certain nombre de choses restent à configurer avant l'activation de
  la pagination. Comme pour le PGD, allouer également une PTB de type `
  (pte32_t*)` à l'adresse `0x601000`.**

**Q5\* : Le but va être maintenant d'initialiser la mémoire virtuelle
  en "identity mapping" : les adresses virtuelles doivent être identiques aux
  adresses physiques. Pour cela :**

* **Bien étudier les plages d'adresses physiques occupées par le noyau
     (`readelf -e kernel.elf`, regarder les program headers).**
* **Préparer au moins une entrée dans le PGD pour la PTB.**
* **Préparer plusieurs entrées dans la PTB.**

**Q6 : Une fois la pagination activée, essayer d'afficher le contenu d'une
  entrée de votre PTB. Que se passe-t-il ? Trouver la solution pour être
  capable de modifier les entrées de votre PTB une fois la pagination
  activée.**

**Q7\* : Avant d'activer la pagination, on souhaiterait faire en sorte que
  l'adresse virtuelle `0xc0000000` permette de modifier votre PGD après
  activation de la pagination. Comment le réaliser ?**


## Quelques exercices supplémentaires de configuration spécifique

**Q8 : Faire en sorte que les adresses virtuelles `0x700000` et `0x7ff000`
  mappent l'adresse physique `0x2000`. Affichez la chaîne de caractères à ces
  adresses virtuelles.**

**Q9 : Effacer la première entrée du PGD. Que constatez-vous ? Expliquez
  pourquoi ?**
