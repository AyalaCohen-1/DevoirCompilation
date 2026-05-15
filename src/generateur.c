#include <stdio.h>
#include <stdlib.h>
#include "generateur.h"

void generer_code_c(Dfa *automate_min, char *expression_originale) {
    FILE *f = fopen("analyseur_lexical.c", "w");
    if (f == NULL) { //mesure de securite au cas ou il y aurait eu un probleme avec la creation 
        perror("Erreur de creation de l'analyseur lexical");
        return;
    }
    //on inclus les librairies de bases necessaires pour le code
    fprintf(f, "#include <stdio.h>\n");
    fprintf(f, "#include <string.h>\n\n");
    fprintf(f, "int main() {\n");
    
    // on demande a l'utilisateur de rentrer un mot a analyser
    fprintf(f, "    char mot[256];\n");
    fprintf(f, "    printf(\"Entrer un mot a tester : \");\n");
    fprintf(f, "    if (scanf(\"%%255s\", mot) != 1) return 1;\n\n");
    
    // On initialise l'état de départ
    fprintf(f, "    int etat = %d;\n\n", automate_min->debut->no);

    // on parcourt le mot
    fprintf(f, "    for (int i = 0; i < strlen(mot); i++) {\n");
    fprintf(f, "        char c = mot[i];\n");
    fprintf(f, "        switch (etat) {\n"); //par rapport a l'etat dans lequel on se trouve le cas et les transitions seront differentes 

    for (int i = 0; i < automate_min->nb_etats; i++) {
        Ensemble_etats *e = automate_min->liste_etats[i];
        fprintf(f, "            case %d:\n", e->no);
        
        TransitionDFA *t = e->liste_transitions;
        if (t != NULL) {
            fprintf(f, "                ");
            int premier = 1;
            while (t != NULL) {
                if (!premier) fprintf(f, " else ");
                fprintf(f, "if (c == '%c') etat = %d;\n", t->lettre, t->destination->no);
                premier = 0;
                t = t->suivante;
            }
            fprintf(f, "                else etat = -1;\n"); 
        } else {
            fprintf(f, "                etat = -1;\n");
        }
        fprintf(f, "                break;\n");
    }

    fprintf(f, "            default: etat = -1; break;\n");
    fprintf(f, "        }\n");
    fprintf(f, "        if (etat == -1) break;\n"); 
    fprintf(f, "    }\n\n");

    // Verifie si la fin du mot correspond a un etat final du dfa_min (sert a savoir si le mot est accepte ou non)
    fprintf(f, "    int est_final = 0;\n");
    for (int i = 0; i < automate_min->nb_etats; i++) {
        if (automate_min->liste_etats[i]->est_final) {
            fprintf(f, "    if (etat == %d) est_final = 1;\n", automate_min->liste_etats[i]->no);
        }
    }

    // affichage du resultat de l'analyse 
    fprintf(f, "\n    if (est_final) {\n");
    fprintf(f, "        printf(\"%%s appartient au langage rationnel defini par %s.\\n\", mot);\n", expression_originale);
    fprintf(f, "    } else {\n");
    fprintf(f, "        printf(\"%%s n'appartient pas au langage rationnel defini par %s.\\n\", mot);\n", expression_originale);
    fprintf(f, "    }\n\n");
    fprintf(f, "    return 0;\n}\n");
    fclose(f);
}