TP0 :

Question 1 :
mbh --> multiboot header
stack --> pile

Adresse mémoire de départ : . = 0x300000;
Selon entry.s, taille de la stack (0x2000)

Selon readelf de kernel.elf :
00	.mbh
01	.stack
02	.idt_jump ...

1. Le point d'entrée est bien à 0x300000, le mbh est LOAD avec 12 octets cela donne 0x30000C (sauf qu'on aligne sur 16, c'est à dire 0x10), ce qui donne 0x300010 en sortie.
2. La stack est ensuite initialisé à partir de 0x300010, et on obtient en sortie 0x302010 (car la taille est de 0x2000).
3. Pour finir, la table des interruptions (idt) est initialisé à partir de 0x302010 et donne en sortie 0x303010, on en déduit 0x1000 la taille de l'idt.

Dans start.c, on voit que la structure de mbh est composé de 3 variable de 32 bits chacune (soit 4 octet chacune, donc 12 octet la taille total).

Après l'idt, on exécute le .text qui correspond au programme.

Schéma :
0x300000 --> MBH (taille 12)
	  |
	  |
0x30000C --> (pas bon car on doit aligné sur 16 dû à la Stack)
0x300010 --> Stack
	  |
	  |
0x302010 --> idt_jmp (taille 0x1000)
	  |
	  |
0x303010 --> .text (program)

##############################


