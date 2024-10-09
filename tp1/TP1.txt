TP1 :
#######################################

Question 1 :
SGDT (Store Global Descriptor Table) --> Instruction assembleur pour récupérer le contenu du GDT (tu reçois l'adresse qui pointe sur GDT).
	SGDT (opérande)
/*
** Global descriptor table
*/
typedef struct global_descriptor_table_register
{
   uint16_t            limit;           /* dt limit = size - 1 */
   union                                /* base address */
   {
      offset_t         addr;
      seg_desc_t       *desc;
   };

} __attribute__((packed)) gdt_reg_t;

La limite ici sert à définir la taille de la GDT.

#######################################

Question 2 :
Bit de granularité : blocs de 4Ko (grand bloc, on multiplie par 4096 bit la taille max), sinon bloc standard sans granularité.

#######################################

Question 3 & 4 :
Selector --> 16 bits
15 --> 3 : Indice du descripteur dans la GDT
2 : G/L
1 --> 0 : RPL (Registre Privilege Level)
On get les valeurs de tout les selecteurs, et on réaliser un décalage de 3 bits sur la droite pour conserver uniquement les valeurs de 15 à 3.

On constate que uniquement CS (Code Selector) pointe sur le descripteur d'indice 1 (de la GDT) et c'est normal, le segment type est défini sur Code.
Contrairement aux autres selecteur qui pointe sur le descripteur d'indice 2 (de la GDT) et c'est normal, le segment type est défini sur Data.

Bien que CS pointe sur un autre descripteur contrairement aux autres selecteurs, ils pointent la même plage d'adresse de mémoire (toute la mémoire actuellement),
donc on est sur un modèle FLAT.

#######################################

