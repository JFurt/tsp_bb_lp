#include <stdlib.h>
#include <stdio.h>

#define LIMIT 1000.

int main(int argc, char *argv[]){
	if(argc < 5){
		printf("Usage: ./%s file_in seed k nR\n",argv[0]);
		exit(-1);
	}

	FILE *fin;
	int k,i,j,l,nR,seed;
	int limite;
	fin = fopen(argv[1],"w");
	if(!fin){
		printf("Erro no arquivo\n");
		exit(-1);
	}
	seed = atoi(argv[2]);
	if(seed<0){
		printf("Erro na seed\n");
		exit(-1);
	}
	k = atoi(argv[3]);
	if(k<0){
		printf("Erro no k\n");
		exit(-1);
	}

	nR = atoi(argv[4]);
	if(nR<0){
		printf("Erro no k\n");
		exit(-1);
	}

	if(nR == 1)
		limite=200;
	else if(nR == 2)
		limite=500;
	else
		limite = 1000;
	srand(seed);
	fprintf(fin,"%d %d %d\n",k,k*k-k,nR);
	fprintf(fin,"0 %d\n",k-1);
   for(i=0;i<nR;i++)
      fprintf(fin,"%d ",limite);
   fprintf(fin,"\n");
	for(i=0;i<k;i++)
		for(j=0;j<k;j++)
			if(i!=j){
				fprintf(fin,"%d %d %d",i,j,(int) (LIMIT*rand()/RAND_MAX));
            for(l=0;l<nR;l++)
               fprintf(fin," %d", (int) (LIMIT/2.*rand()/RAND_MAX));
            fprintf(fin,"\n");
         }
	fclose(fin);
	return 1;
}
