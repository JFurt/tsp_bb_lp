#include "subgradiente.h"
#include <glpk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "utilidades.h"
double subgradiente(ProblemaRaiz *raiz, double *saida, int *opt, int maxIter){
	double lowerBound, upperBound,zlb;
	//double mi=0,Hk,T;
	int i,sum,cont=1,j,b_valid;
	double T,Vzlb,prodH;
	double *mi, *H;
	double *s = calloc(raiz->nA,sizeof(double));
	mi = calloc(raiz -> nR, sizeof(double));
	H = calloc(raiz -> nR, sizeof(double));
	lowerBound=-DBL_MAX; upperBound=DBL_MAX;
   glp_prob *lp;
   lp = glp_create_prob();
   montaGLPK(lp,raiz);
 	glp_set_obj_dir(lp,GLP_MIN);


	while(lowerBound != upperBound && cont <= maxIter){
		for(i=0;i<raiz->nA;i++){
		   sum=0;
		   for(j=0;j<raiz->nR;j++)
   			sum += mi[j]*raiz->consumo[j][i]; //USAR FOR
			glp_set_obj_coef(lp,i+1,raiz->custo[i] + sum);
		}

		glp_simplex(lp,raiz->parm);
		if(GLP_NOFEAS == glp_get_prim_stat(lp)){//SEM SOLUCAO
			upperBound=DBL_MAX;
			break;
		}

		zlb = glp_get_obj_val(lp);
		for(j=0;j<raiz->nR;j++)
		   zlb -= mi[j]*raiz->recDisp[j];


      b_valid=1;
      prodH=0;
      for(j=0;j<raiz->nR;j++){
		   H[j] = - raiz->recDisp[j];
		   for(i=0;i<raiz->nA;i++){
			   s[i] = glp_get_col_prim(lp,i+1);
			   if(s[i]!=1 && s[i]!=0)
				   break;
			   H[j] += raiz->consumo[j][i]*s[i];
		   }
		   prodH += H[j]*H[j];
		   if(H[j] > 0)
		      b_valid=0;
		}

      Vzlb = 0;
      for(i=0;i<raiz->nA;i++){
		   Vzlb += s[i]*raiz->custo[i];
		}

		if(zlb > lowerBound){
			lowerBound=zlb;
		}

		if(b_valid && Vzlb < upperBound){
			upperBound = Vzlb;
			memcpy(saida,s,raiz->nA * sizeof(double));
		}

		if(upperBound!=DBL_MAX)
			T = 0.25*(upperBound - zlb) / (prodH);
		else
			T = 1./cont;
      for(j=0;j<raiz->nR;j++)
   		mi[j] = MAX(0,mi[j] + T*H[j]);

		cont++;

		//printf("%d - lower=%lf, zlb=%lf, Vzlb=%lf, upper=%lf, Hk=%lf, theta=%lf, mi=%lf\n",cont,lowerBound,zlb,Vzlb,upperBound,Hk,T,mi);
	}
	glp_erase_prob(lp);
	if(lowerBound==upperBound)
		*opt=1;
	else
		*opt=0;
	if(upperBound != DBL_MAX)
		return upperBound;
	else
		return -1;
}

double subgradienteSubRotina(glp_prob *lp, ProblemaRaiz *raiz, ProblemaNodo *no,double *solucao,int limSup,int maxIter){
	int brk,cont,i,j;
	double theta, delta;
	double *ajuste, *mi;
   double vr,ar,alfa_k;

   if(maxIter==0)
      return -1;

   for(i=0;i<raiz->nA;i++){
		if(no -> mSelecao[i] == 1)
			glp_set_col_bnds(lp,i+1, GLP_FX, 1., 1.);
		else if(no -> mSelecao[i] == -1)
			glp_set_col_bnds(lp,i+1, GLP_FX, 0., 0.);
   }

	mi = calloc(raiz->nA, sizeof(double));
	ajuste = calloc(raiz -> nA, sizeof(double));
	memset(ajuste,0, raiz->nA*sizeof(double));
	cont=0;brk=0;
	alfa_k=1;
	do{
		cont++;//theta=100./cont;

		vr=0;
		ar =0;
	   for(i=0;i< raiz -> nA; i++){
		   solucao[i] = glp_get_col_prim(lp,i+1); //da p osmar o b*mi ao resultado?
         vr += solucao[i]*raiz->custo[i];
			ar += ajuste[i]*ajuste[i];
	   }
		if(cont > 1)
			theta = alfa_k*(limSup-vr)/ar;
		else
			theta = 1 ;
      //printf("altura=%d\n",cont);
		printf("%lf\n",theta);
		for(i=0;i<raiz->nA;i++)
			glp_set_obj_coef(lp,i+1,raiz->custo[i] + ajuste[i]);

		glp_simplex(lp,NULL);    
		for(i=0;i<raiz->nA;i++)
			if(glp_get_col_prim(lp,i+1) ==1.)
				printf("%d - %lf, custo = %d, consumo = %d\n",i,glp_get_col_prim(lp,i+1),raiz->custo[i], raiz->consumo[0][i]);
		brk=1;
		for(i=0;i<raiz->nR;i++){
			delta = raiz -> recDisp[i];
			for(j=0;j<raiz->nA;j++)
				delta -= glp_get_col_prim(lp,j+1)*raiz->consumo[i][j];
			mi[i] = MAX(mi[i] - theta * delta,0);
			//printf("%lf, %lf\n",mi[i], delta);
			if(delta<0)
				brk=0;
		}

		if(!brk)
			for(i=0;i<raiz->nA;i++){
				ajuste[i] = 0;
				for(j=0;j<raiz->nR;j++)
					ajuste[i] += mi[j]*raiz->consumo[j][i];
			}
	}while(!brk && cont < maxIter);
   vr=0;
   if(!brk || GLP_NOFEAS == glp_get_prim_stat(lp))
      vr=-1;
   else
	   for(i=0;i< raiz -> nA; i++){
		   solucao[i] = glp_get_col_prim(lp,i+1);
         vr += solucao[i]*raiz->custo[i];
	   }
	double v;
	for(i=0;i< raiz -> nA; i++){
		v = solucao[i]-1.;
		if(v*v <= ERRO*ERRO)
			printf("Usando aresta %d,valor=%lf, custo= %d, consumo = %d\n",i,solucao[i],raiz->custo[i], raiz->consumo[0][i]);
	}
	double bx;
	bx=0;
	for(i=0;i<raiz->nR;i++){
		bx += raiz->recDisp[i]*mi[i];
		printf("mi[%d]=%lf, recDisp[%d]=%d\n",i,mi[i],i,raiz->recDisp[i]);
	}
	printf("%lf\n",glp_get_obj_val(lp));
   free(mi); free(ajuste);
   return vr;
}

