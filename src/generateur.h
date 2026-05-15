#ifndef GENERATEUR_H
#define GENERATEUR_H
#include "dfa.h"
#include "minimisation_dfa.h"

void generer_code(Dfa *automate_min, char *expression);

#endif