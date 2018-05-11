#ifndef utilidades_h_GUARD
#define utilidades_h_GUARD
#include "estruturas.h"
#include <glpk.h>
void montaGLPK(glp_prob *lp, ProblemaRaiz *raiz);
void inicializaRaiz(ProblemaRaiz *raiz, int **lArco, int nV, int nA,int *recDisp, int nR,int origem, int destino);
void montaGLPKLinear(glp_prob *lp, ProblemaRaiz *raiz);


#endif
