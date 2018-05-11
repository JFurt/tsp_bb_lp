#ifndef subgradiente_h_GUARD
#define subgradiente_h_GUARD

#include "estruturas.h"

void subgradienteResolvedor(ProblemaRaiz *raiz, ProblemaNodo *no);
double subgradienteSubRotina(glp_prob *lp, ProblemaRaiz *raiz, ProblemaNodo *no,double *solucao,int limSup,int maxIter);


double subgradiente(ProblemaRaiz *raiz, double *saida, int *opt, int maxIter);
#endif
