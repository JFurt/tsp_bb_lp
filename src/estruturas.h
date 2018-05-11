#ifndef estruturas_h_GUARD
#define estruturas_h_GUARD
#include <glpk.h>
#define REL_LAGR 1
#define REL_LINEAR 0
#define MAX(A,B) A > B ? A : B
#define MIN(A,B) A < B ? A : B
#define ERRO 1e-10
#define FIFO 1
#define PILHA 2
typedef struct ProblemaRaiz{
	 int **MI; // nV x nA
	 int *b; //nV
	 int nV;
	 int nA;

	 int partida;
	 int chegada;

	 int *custo; // nA
	 int **consumo; // nA x nR
	 int nR;
	 int *recDisp; //nR

		glp_smcp *parm;

   int ordemLista;
	int mxLagr;

}ProblemaRaiz;

typedef struct  ProblemaNodo{
	 int *mSelecao; // nA, -1 = nao decidido, 0 = decidido p zero, 1 = decidido p um
	 int nS;
	 struct ProblemaNodo *prox;
	 struct ProblemaNodo *ante;
    int *consumoTotal;
    int valorTotal;
    int contadorLagr;
}ProblemaNodo;

typedef struct RelLagrange{
	double *mi; //nR
	double *ajuste; //nA
}RelLagrange;

typedef struct ParamRelax{ //se precisar usar union
	int tipo;
	RelLagrange *rel;
}ParamRelax;




#endif
