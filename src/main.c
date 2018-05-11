#include <stdlib.h>
#include <stdio.h>
#include "estruturas.h"
#include "utilidades.h"
#include "subgradiente.h"
#include "bb.h"
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <float.h>

int main(int argc, char *argv[]){
	int i,j;
	int nV,nA,nR, origem, destino;
	int **lArco;
	int *recDisp;
	double boundSubgradiente, boundBB, *solucaoSubgradiente, *solucaoBB;
	int opt;

	ProblemaRaiz raiz;
	glp_smcp parm;
   FILE *fin;

   if(argc < 3){
      printf("Use: ./%s <arquivo_entrada> <FIFO | PILHA>\n",argv[0]);
      exit(-1);
   }
   fin = fopen(argv[1],"r");
   if(!fin){
      printf("Arquivo de entrada incorreto: %s\n",argv[1]);
      exit(-1);
   }
   if(!strcmp("FIFO", argv[2]))
      raiz.ordemLista = FIFO;
   else if(!strcmp("PILHA", argv[2]))
      raiz.ordemLista = PILHA;
   else{
      printf("Tipo <FIFO|PILHA> incorreto\n");
      exit(-1);
   }



	glp_init_smcp(&parm);
	parm.msg_lev = GLP_MSG_ERR;
	raiz.parm = &parm;
   
   
   
	printf("Lendo dados....\n");

	fscanf(fin," %d %d %d", &nV, &nA, &nR);
	fscanf(fin," %d %d",&origem, &destino);

	recDisp=calloc(nR,sizeof(int));
	for(i=0;i<nR;i++)
		fscanf(fin," %d", recDisp +i);
	lArco = malloc(nA*sizeof(int *));
	for(i=0;i<nA;i++){
		lArco[i] = malloc((3 + nR)*sizeof(int));
		fscanf(fin," %d %d %d",lArco[i], lArco[i]+1,lArco[i]+2);
		for(j=0;j<nR; j++)
			fscanf(fin," %d",lArco[i]+3+j);
	}
   fclose(fin);
   printf("%s\n",argv[1]);
	printf("Instância V=%d, R=%d, tipo=%s\n",nV,nR,argv[2]);
	printf("Iniciando Matriz de Incidência...\n");
	inicializaRaiz(&raiz,lArco,nV,nA,recDisp,nR,origem,destino);
	solucaoSubgradiente = calloc(nA,sizeof(double));
	solucaoBB = calloc(nA,sizeof(double));

   struct timeval tt1,tt2;
   


	printf("Resolvendo problema...\n");
	gettimeofday(&tt1,NULL);
	//boundSubgradiente=subgradiente(&raiz,solucaoSubgradiente,&opt,100);
	//printf("limitante obtido pelo subgradiente %lf\n",boundSubgradiente);
    boundSubgradiente=DBL_MAX;
    opt=0;
	if(!opt)
		boundBB = branchandbound(&raiz,boundSubgradiente,solucaoBB);
   gettimeofday(&tt2,NULL);
	long diff = (tt2.tv_sec-tt1.tv_sec)*1000000 + tt2.tv_usec-tt1.tv_usec;


   double *solucao_otima ;
	int in, out;
	if(boundBB < boundSubgradiente){
		printf("Problema resolvido através de branch and bound.\nValor otimo %lf\n",boundBB);
		solucao_otima = solucaoBB;
	}else{
		printf("Problema resolvido através de subgradiente. \nValor otimo %lf\n",boundSubgradiente);
		solucao_otima = solucaoSubgradiente;
	}
	for(i=0;i< raiz.nA; i++){
		if(solucao_otima[i]==1.){
         for(j=0;j<raiz.nV;j++){
            if(raiz.MI[j][i] == 1)
               out=j;
            if(raiz.MI[j][i]==-1)
               in=j;
         }
			printf("Usando aresta %d=(%d,%d), custo = %d",i,out,in,raiz.custo[i]);
			for(j=0;j<raiz.nR;j++)
			   printf(", consumo%d = %d",j,raiz.consumo[j][i]);
			printf("\n");
		}
	}

   printf("Tempo: %lf\n", ((double) diff)/1000000.);

   FILE *f_time;
   char *str = malloc(100*sizeof(char));
   sprintf(str,"resultados/%c_tempo.dat",argv[2][0]);
   f_time=fopen(str,"a+");
   if(!f_time){
      printf("Erro na saida\n");
      exit(-1);
   }
   fprintf(f_time,"%d\t%lf\n",raiz.nV,  ((double) diff)/1000000.);
   fclose(f_time);


	return 0;
}
