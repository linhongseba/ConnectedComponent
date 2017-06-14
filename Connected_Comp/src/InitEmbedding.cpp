#include"IOfunction.h"
#include"Graphstat.h"
/*
@author: Linhong Zhu (linhong.seba.zhu@gmail.com)
@last_update_time: April 14, 2016
@bug report: please create issues in GitHub.
copyright 1.0 Linhong Zhu
under Apache 2.0 licence
*/

int int_to_bin_digit(unsigned int in, int count, int* out){
    /* assert: count <= sizeof(int)*CHAR_BIT */
    unsigned int mask = 1U << (count-1);
    int i;
    int deg = 0;
    for (i = 0; i < count; i++) {
        out[i] = (in & mask) ? 1 : 0;
        if (out[i] == 1) {
        	deg++;
        }
        in <<= 1;
    }
    return deg;
}

void printEmbeddingDeg(int i, int deg, int * mask, int length, int pos, FILE *wfile) {
	fprintf(wfile, "%d,%d", i, deg);
	for (int j = 0; j < length; j++) {
		if (mask[j] == 1) {
			double weight = 1.0/deg;
			fprintf(wfile, ":%d,%lf", j + pos, weight);
		}
	}
	fprintf(wfile, "\n");
}

void printEmbeddingZero(int i, int dim, int ccindex, int ccsize, FILE *wfile) {
	fprintf(wfile, "%d,%d", i, 1);
	double weight = 1.0/(ccsize + 1);
	int idx = ccindex%dim + (int)rand()%dim;
	fprintf(wfile, ":%d,%lf", idx, weight);
	fprintf(wfile, "\n");
}

int main(int argc, char *argv[]){
	if(argc<2){
		printf("Usage: %s [input_graph_file][dimension] [output_file]\n", argv[0]);
		exit(0);
	}
	Initmemory();
	InitIOmemory();
	FILE *rfile;

	//read the new graph (start)//
	rfile = fopen(argv[1], "r");
	if (rfile == NULL){
		printf("could not open file to read\n");
		exit(1);
	}
	int n;
	fscanf(rfile, "%d\n", &n);
	Node *G;
	ReadGraph(G, n, rfile);
	fclose(rfile);
	//read the new graph (end)//

    // compute the connected component (start) //
	ccid = new int[n];
	memset(ccid,0,sizeof(int)*n);
	isvisited = new char[n];
	int ccnum = ConnectedComp(G, n);
	printf("total number of connected component is %d\n",ccnum);
	if(isvisited != NULL)
		delete[]isvisited;
	if(visitstack != NULL)
		free(visitstack);
	int * ccsize = new int[ccnum + 1];
	memset(ccsize, 0, sizeof(int) *(ccnum +1));
	for(int i = 0;i < n; i++){
		int ccindex = ccid[i];
		ccsize[ccindex]++;
	}
	// compute the connected component (end) //


	// Initilize the embedding and save to file (start)
	int dim = atoi(argv[2]);
	int length = dim / 2 ;
	int *mask = new int[length];
	memset(mask, 0, sizeof(int)*length);
	FILE *wfile = fopen(argv[3],"w");
	if(wfile == NULL){
		printf("could not open file to write\n");
		exit(3);
	}
	fprintf(wfile, "%d\n", n);
	for(int i = 0; i < n; i++) {
		unsigned int hash = (unsigned int)ccid[i];
		int pos = 0;
		if (ccsize[hash] > 500) {
			hash += rand()%dim;
			pos = dim - length;
		}
		int deg = int_to_bin_digit(hash, length, mask);
		if (deg > 0) {
			printEmbeddingDeg(i, deg, mask, length, pos, wfile);
		} else {
			printEmbeddingZero(i, dim, ccid[i], ccsize[ccid[i]], wfile);
		}
		memset(mask, 0, sizeof(int)*length);
	}
	if(wfile != NULL) {
		fclose(wfile);
	}
	// Initilize the embedding and save to file (end)
	if(ccid != NULL) {
		delete []ccid;
	}
	if(ccsize != NULL) {
		delete []ccsize;
	}
	releasegraphmemory(G, n);
	if (G != NULL){
		free(G);
		G = NULL;
	}
	releaseIOmemory();
	releaseblockmemory();
	return 0;
}
