#include "utilidades.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void montaGLPK(glp_prob *lp, ProblemaRaiz *raiz){
	int i,j;
	int *vx, *vy;
	double *vc;
	int size;

	char *strNome;
	strNome = malloc(5*sizeof(char));

   glp_set_obj_dir(lp,GLP_MIN);

	glp_add_rows(lp,raiz->nV);

	for(i=0;i< raiz->nV; i++){
		sprintf(strNome,"v%d",i);

		glp_set_row_name(lp,i+1,strNome);
		glp_set_row_bnds(lp,i+1, GLP_FX, raiz -> b[i] , raiz -> b[i]);		
	}
	glp_add_cols(lp,raiz->nA);
	for(i=0;i < raiz -> nA; i++){
		sprintf(strNome,"x%d",i);

		glp_set_col_name(lp, i+1,strNome);
		glp_set_col_bnds(lp,i+1, GLP_DB, 0., 1.);
	}

	size = raiz -> nV * raiz -> nA + 1;
	vx = malloc(size * sizeof(int));
	vy = malloc(size * sizeof(int));
	vc = malloc(size * sizeof(double));
	for(i=0;i< raiz -> nV; i++){
		for(j=0;j < raiz -> nA; j++){
			vx[i*raiz -> nA + j + 1] = i+1;
			vy[i*raiz -> nA + j + 1] = j+1;
			vc[i*raiz -> nA + j + 1] = (double) raiz -> MI[i][j];
			//printf("[%d,%d]~%d, %d, %d, %d\n",i,j,i*raiz->nA + j + 1,i+1,j+1,raiz->MI[i][j]);
		}
	}
	glp_load_matrix(lp, size-1, vx,vy,vc);
}

void montaGLPKLinear(glp_prob *lp, ProblemaRaiz *raiz){
	int i,j;
	int *vx, *vy;
	double *vc;
	int size;

	char *strNome;
	strNome = malloc(5*sizeof(char));

   glp_set_obj_dir(lp,GLP_MIN);

	glp_add_rows(lp,raiz->nV);
	for(i=0;i< raiz->nV; i++){
		sprintf(strNome,"v%d",i);

		glp_set_row_name(lp,i+1,strNome);
		glp_set_row_bnds(lp,i+1, GLP_FX, raiz -> b[i] , raiz -> b[i]);		
	}
	if(raiz -> nR > 0){
		glp_add_rows(lp,raiz->nR);
		for(i=0;i< raiz->nR; i++){
			sprintf(strNome,"r%d",i);

			glp_set_row_name(lp,i+1 + raiz -> nV,strNome);
			glp_set_row_bnds(lp,i+1 + raiz -> nV, GLP_UP, 0.0 , raiz -> recDisp[i]);		
		}
	}

	glp_add_cols(lp,raiz->nA);
	for(i=0;i < raiz -> nA; i++){
		sprintf(strNome,"x%d",i);

		glp_set_col_name(lp, i+1,strNome);
		glp_set_col_bnds(lp,i+1, GLP_DB, 0., 1.);
		glp_set_obj_coef(lp,i+1,raiz->custo[i]);
	}

	size = (raiz -> nV + raiz -> nR) * raiz -> nA + 1;
	vx = malloc(size * sizeof(int));
	vy = malloc(size * sizeof(int));
	vc = malloc(size * sizeof(double));
	for(i=0;i< raiz -> nV; i++){
		for(j=0;j < raiz -> nA; j++){
			vx[i*raiz -> nA + j + 1] = i+1;
			vy[i*raiz -> nA + j + 1] = j+1;
			vc[i*raiz -> nA + j + 1] = (double) raiz -> MI[i][j];
			//printf("[%d,%d]~%d, %d, %d, %d\n",i,j,i*raiz->nA + j + 1,i+1,j+1,raiz->MI[i][j]);
		}
	}
	for(i=raiz -> nV;i<(raiz -> nV + raiz-> nR);i++){
		for(j=0;j < raiz -> nA; j++){
			vx[i*raiz -> nA + j + 1] = i+1;
			vy[i*raiz -> nA + j + 1] = j+1;
			vc[i*raiz -> nA + j + 1] = (double) raiz -> consumo[i-raiz->nV][j];
			//printf("[%d,%d]~%d, %d, %d, %d\n",i,j,i*raiz->nA + j + 1,i+1,j+1,raiz->consumo[i-raiz->nV][j]);
		}
	}
	glp_load_matrix(lp, size-1, vx,vy,vc);
}


void inicializaRaiz(ProblemaRaiz *raiz, int **lArco, int nV, int nA,int *recDisp,int nR,int origem, int destino){
	int i,j;
	raiz -> nV = nV;
	raiz -> nA = nA;
	raiz -> nR = nR;
	printf("Alocando matriz de incidência...\n");
	raiz -> MI = calloc(nV,  sizeof(int *));
	for(i=0;i< raiz ->nV;i++){
		//printf("%d\n",i);
		raiz -> MI[i] = calloc(nA, sizeof(int));
	}

	raiz -> b = calloc(nV, sizeof(int));
	raiz -> custo = calloc(nA, sizeof(int));

	raiz -> consumo = calloc(nR, sizeof(int *));
	for(i=0;i<nR;i++)
		raiz -> consumo[i] = calloc(nA, sizeof(int));

	raiz -> recDisp = calloc(nR, sizeof(int));
	memcpy(raiz->recDisp,recDisp,nR*sizeof(int));
	printf("Setando matriz de incidência...\n");
	for(i=0;i<nA;i++){
		raiz -> MI[lArco[i][0]][i] = 1;
		raiz -> MI[lArco[i][1]][i] = -1;
		raiz -> custo[i] = lArco[i][2];
		for(j=0;j<nR;j++)
			raiz->consumo[j][i] = lArco[i][3+j];
	}
	raiz -> b[origem] = 1;
	raiz -> b[destino] = -1;

	/*printf("b=");
	for(i=0;i<nV;i++)
		printf("%d ",raiz ->b[i]);
	printf("\n");*/
}
