#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE *fin, *fout;
	fpos_t pos;
	int nInduc, nInducA, i, v1,v2,c,nV,nA;
	if(argc < 4){
		printf("Usage: ./%s file_in file_out n_induction\n",argv[0]);
		exit(-1);
	}

	fin = fopen(argv[1], "r");
	if(!fin){
		printf("Invalid file_in, %s\n",argv[1]);
		exit(-1);
	}

	fout = fopen(argv[2], "w+");
	if(!fin){
		printf("Invalid file_out, %s\n",argv[2]);
		exit(-1);
	}

	nInduc = atoi(argv[3]);
	if(nInduc <= 0){
		printf("Invalid n_induction\n");
		exit(-1);
	}
		
	nInducA= 0;
	fscanf(fin," %d %d",&nV,&nA);
	fgetpos(fout,&pos);
	fprintf(fout,"          \n");
	for(i=0;i<nA;i++){
		fscanf(fin," %d %d %d",&v1, &v2, &c);
		if(v1 < nInduc && v2 < nInduc){
			nInducA++;
			fprintf(fout,"%d %d %d\n",v1,v2,c);
		}
	}
	fsetpos(fout,&pos);
	fprintf(fout,"%d %d",nInduc,nInducA);
	return 0;
}
