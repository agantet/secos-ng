# TP3 - Les niveaux de privilèges

Le but du TP3 est de bien comprendre les niveaux de privilèges et de réussir à
démarrer du code en ring 3.

Comme abordé en cours, ce chapitre de la documentation des processeurs Intel
est probablement le plus complexe. Le TP ne couvre pas tous les aspects des
transitions de niveaux de privilèges.

L'idée est ici de réussir à démarrer une tâche utilisateur en détournant
l'usage principal de l'instruction IRET pour passer d'un contexte ring 0 à un
contexte ring 3.

**Q1\* : reprendre la GDT telle que définie à la question [Q12](../tp1/README.md) du TP1
   (notamment avec les 2 descripteurs de droits ring 3).**

**Q2\* : à l'aide de l'instruction PUSH, en assembleur inline, préparer la pile
  comme le CPU l'aurait fait à l'arrivée d'une interruption, à savoir, dans
  l'ordre, empiler :**

* la valeur de SS
* la valeur de ESP
* la valeur de EFLAGS
* une valeur fictive de CS : un sélecteur de descripteur de segment de code ring 3
* une valeur fictive de EIP : l’adresse de début du code ring 3 qu’on cherche à exécuter (`userland()`).

**Q3\* : Rajouter alors à votre code inline précédent l'instruction IRET.
  S'assurer que le passage au ring 3 s'est bien effectué, notamment en
  observant une #GP lors de l'exécution du `mov eax, cr0`.**