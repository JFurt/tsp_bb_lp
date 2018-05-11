#include "bb.h"
#include "utilidades.h"
#include "subgradiente.h"
#include <glpk.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>



double branchandbound(ProblemaRaiz *raiz, double iniBound, double *solucaoFinal){
	ProblemaNodo *P,*P1,*P2;
	double *solucao = malloc(raiz -> nA*sizeof(double));
	double *solucao_otima = malloc(raiz -> nA*sizeof(double));
	double bound,limSup = DBL_MAX;

	int bInt,bInvalido,i, cut ;

   //int limiteExecSubgrad;

	Lista *F = malloc(sizeof(Lista));
   initLista(F);
	glp_prob *lp;

	lp = glp_create_prob();

	montaGLPKLinear(lp,raiz);


	if(iniBound > 0)
		limSup = iniBound;

   P1= malloc(sizeof(ProblemaNodo));
   P1 -> mSelecao = calloc(raiz->nA,sizeof(int)); P1 -> nS = 0;
   P1 -> consumoTotal = calloc(raiz->nR, sizeof(int));
   P1 -> valorTotal = 0;


	pushLista(F,P1);
	while((P = popLista(F,raiz))){
	   bInvalido=0;
	   for(i = 0; i < raiz -> nR; i++)
	      if(P -> consumoTotal[i] > raiz -> recDisp[i])
	         bInvalido=1;
      if(!bInvalido && P -> valorTotal <= limSup){//corte preliminar
		   //verifica se é viavel e calcula bounds
		   bound = solveGLPKLinear(lp,raiz,P,solucao,&bInt);
		   if(bInt && limSup > bound){//solucao inteira, atualiza limitante
			   limSup = bound;
			   memcpy(solucao_otima,solucao,raiz->nA*sizeof(double));
		   }
		   //corta
			cut =(bInt == 1) || (bound < 0) || (limSup <= bound) || (P->nS >= raiz->nA);
			if(!cut){
			   P1 = malloc(sizeof(ProblemaNodo)); P2 = malloc(sizeof(ProblemaNodo));

			   P1 -> mSelecao = calloc(raiz -> nA, sizeof(int)); memcpy(P1 -> mSelecao, P -> mSelecao, raiz -> nA * sizeof(int));
			   P1 -> nS = P -> nS + 1; P1 -> mSelecao[P -> nS] = 1; 
			   P1 -> consumoTotal = calloc(raiz -> nR, sizeof(int));
			   memcpy(P1 -> consumoTotal,P->consumoTotal,raiz->nR * sizeof(int));
			   for(i=0;i<raiz->nR;i++)
			      P1 -> consumoTotal[i] += raiz -> consumo[i][P->nS];
            P1 -> valorTotal = P -> valorTotal + raiz->custo[P->nS];
            
			   P2 -> mSelecao = calloc(raiz -> nA, sizeof(int));  memcpy(P2 -> mSelecao, P -> mSelecao, raiz -> nA * sizeof(int));
			   P2 -> nS = P -> nS + 1; P2 -> mSelecao[P -> nS] = -1;
			    
			   P2 -> consumoTotal = calloc(raiz -> nR, sizeof(int));
            memcpy(P2 -> consumoTotal,P -> consumoTotal,raiz->nR*sizeof(int)); 
            
            P2 -> valorTotal = P -> valorTotal;

			   pushLista(F,P2); pushLista(F,P1);
		   }
      }
      free(P -> mSelecao); 
      free(P);
	}
	free(F);
	free(solucao);
	memcpy(solucaoFinal, solucao_otima, raiz->nA* sizeof(double));

   free(solucao_otima);
   glp_erase_prob(lp); 
	return limSup;   
}


void initLista(Lista *F){
	F -> primeiro = NULL;
	F -> ultimo = NULL;
}

void pushLista(Lista *F, ProblemaNodo *P){
	P -> prox = F -> ultimo; P -> ante = NULL;
	if(!F -> primeiro || !F -> ultimo){ //lista vazia
		F -> primeiro = P;	
	}else
		F -> ultimo -> ante = P;
	F -> ultimo = P;
}
ProblemaNodo *popLista(Lista *F, ProblemaRaiz *raiz){
   if(raiz -> ordemLista == FIFO)
      return popFIFO(F);
   else if(raiz -> ordemLista == PILHA)
      return popPILHA(F);
   else 
      return NULL;
}


ProblemaNodo *popFIFO(Lista *F){ //Lista
	ProblemaNodo *P;
	P = F -> primeiro;
	if(P){
		F -> primeiro = P -> ante;
	   P -> ante = NULL;P -> prox=NULL;
	}

	return P;
}

ProblemaNodo *popPILHA(Lista *F){ //PILHA
	ProblemaNodo *P;
	P = F -> ultimo;
	if(P){
		F -> ultimo = P -> prox;
	   P -> ante = NULL;P -> prox=NULL;
	}

	return P;
}
double solveGLPKLinear(glp_prob *lp, ProblemaRaiz *raiz, ProblemaNodo *no,double  *solucao, int *bInt){
	int i;

	for(i=0;i<raiz->nA;i++){
		if(no -> mSelecao[i] == 1){
			glp_set_col_bnds(lp,i+1, GLP_FX, 1., 1.);
      }
		else if(no -> mSelecao[i] == -1){
			glp_set_col_bnds(lp,i+1, GLP_FX, 0., 0.);
		}
		else
			glp_set_col_bnds(lp,i+1, GLP_DB, 0., 1.);
	}
	glp_simplex(lp, raiz->parm);
	if(GLP_NOFEAS == glp_get_prim_stat(lp)){//SEM SOLUCAO
		*bInt = 0;
		memset(solucao,0,raiz->nA*sizeof(double));
		return -1;
	}
	*bInt = 1;
	for(i=0;i<raiz->nA;i++){
		solucao[i] = glp_get_col_prim(lp,i+1);
		if( (int) solucao[i] - solucao[i] > ERRO || (int) solucao[i] - solucao[i] < -ERRO)
			*bInt = 0;
	}
	return glp_get_obj_val(lp);

}
