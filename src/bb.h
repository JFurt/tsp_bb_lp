#ifndef BB_H_GUARD
#define BB_H_GUARD

#include "estruturas.h"
#include <glpk.h>
typedef struct Lista{
	ProblemaNodo *primeiro;
	ProblemaNodo *ultimo;
}Lista;

double branchandbound(ProblemaRaiz *raiz, double iniBound, double *solucaoFinal);
void initLista(Lista *F);
void pushLista(Lista *F, ProblemaNodo *P);
ProblemaNodo *popLista(Lista *F, ProblemaRaiz *raiz);
ProblemaNodo *popFIFO(Lista *F);
ProblemaNodo *popPILHA(Lista *F);
double solveGLPKLinear(glp_prob *lp, ProblemaRaiz *raiz, ProblemaNodo *no,double  *solucao, int *bInt);
#endif
