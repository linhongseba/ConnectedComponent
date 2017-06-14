#include"IOfunction.h"
#include"Graphstat.h"
/*
A simple implementation for the connected componennt algorithm
@author: Linhong Zhu (linhong.seba.zhu@gmail.com)
@last_update_time: April 14, 2016
@bug report: please create issues in GitHub.
copyright 1.0 Linhong Zhu
under Apache 2.0 licence
*/
int main(int argc, char *argv[]){
	if(argc<2){
		printf("Usage: %s [input_graph_file] [output_file]\n", argv[0]);
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
	//read the new graph (start)//
	Node *G;
	ReadGraph(G, n, rfile);
	fclose(rfile);
	//PrintGraph(G,n);
	//read the new graph (end)//
	int m = 0;
	for (int i = 0; i < n; i++){
		m += G[i].deg;
	}
	m = m / 2;
	cout << "number of edges " << m << endl;
	ccid=new int[n];
	memset(ccid,0,sizeof(int)*n);
	isvisited=new char[n];
	int ccnum=ConnectedComp(G, n);
	FILE *wfile=fopen(argv[2],"w");
	if(wfile==NULL){
		printf("could not open file to write\n");
		exit(3);
	}
	for(int i=0;i<n;i++){
		fprintf(wfile,"%d\t%d\n",i,ccid[i]);
	}
	if(wfile!=NULL)
		fclose(wfile);
	printf("total number of connected component is %d\n",ccnum);
	if(isvisited!=NULL)
		delete[]isvisited;
	if(visitstack!=NULL)
		free(visitstack);
	if(ccid!=NULL)
		delete []ccid;
	releasegraphmemory(G, n);
	if (G != NULL){
		free(G);
		G = NULL;
	}
	releaseIOmemory();
	releaseblockmemory();
	return 0;
}
