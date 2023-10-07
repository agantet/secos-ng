# TP 5 - Les appels systèmes

Le but du TP est de bien comprendre les appels systèmes. La ligne
d'interruption 48 n'étant utilisé par aucune exception ni autre usage dans ce
projet, on se propose de définir une routine de traitement d'appels système
sur cette ligne. 

**Q1\* : Un squelette de fonction `userland` est fourni dans `tp.c`. Reprendre
  le code du TP 3 pour modifier `tp()` de manière à démarrer du code ring
  3.**

La routine suivante est fournie dans `tp.c` :
```c
void syscall_isr()
{
   asm volatile (
      "leave ; pusha        \n"
      "mov %%esp, %eax      \n"
      "call syscall_handler \n"
      "popa ; iret"
      );
}

void __regparm__(1) syscall_handler(int_ctx_t *ctx)
{
   debug("SYSCALL eax = %p\n", ctx->gpr.eax);
}
```

**Q2 : En s'inspirant du TP2, mettre à jour les offsets du descripteur 48 de
  l'IDT pour installer la fonction `syscall_isr()` comme gestionnaire de
  l'interruption 48.**

**Q3 : Essayer de déclencher l'interruption "48" depuis le ring 3 de manière
  logicielle avec l'instruction INT par exemple comme suit :**

```c
void userland() {
   uint32_t arg =  0x2023;
   asm volatile ("int $48"::"a"(arg));
   while(1);
}
```

**Q3\* : Pourquoi observe-t-on une #GP ? Corriger le problème de sorte qu'il
  soit autorisé d'appeler l'interruption "48" avec un RPL à 3.**

**Q4\* : Modifier la fonction `syscall_handler()` pour qu'elle affiche une
  chaîne de caractères dont l'adresse se trouve dans le registre "ESI". Nous
  venons de créer un appel système permettant d'afficher un message à l'écran
  et prenant son argument via "ESI". Essayer cet appel système depuis votre
  fonction `userland()`.**

**Q5 : Quel problème de sécurité y a-t-il à l'implémentation de
  `syscall_handler()` ? Essayez de pirater ce service, depuis `userland
  ()`, afin de lire de la mémoire du noyau. Modifier le code de
  `syscall_handler` pour corriger ce problème.**
