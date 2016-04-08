#include"IOfunction.h"
char *isvisited;
int *visitstack;
int *ccid;
int stacksize;
void DFS(Node *G, int n, int v, int ccnum){
	if(visitstack==NULL){
		visitstack=(int*)malloc(sizeof(int)*n);
		stacksize=n;
	}
	int pos=0;
	visitstack[pos]=v;
	isvisited[v]='g';
	pos++;
	while(pos>0){
		int u=visitstack[pos-1];
		if (isvisited[u]=='b'){
			pos--;
			continue;
		}
		bool flag=true;
		for(int i=0;i<G[u].deg;i++){
			int nb=G[u].nbv[i];
			if(isvisited[nb]=='v'){
				if(pos>=stacksize){
					stacksize+=1000;
					visitstack=(int*)realloc(visitstack,sizeof(int)*stacksize);
					if(visitstack==NULL){
						printf("system could not allocate more memory\n");
						exit(4);
					}
				}
				visitstack[pos]=nb;
				pos++;
				isvisited[nb]='g';
				flag=false;
			}
		}
		if(flag==true){
			isvisited[u]='b';
			ccid[u]=ccnum;
		}
	}
}

int ConnectedComp(Node *G, int n){
 	memset(isvisited, 'v',sizeof(char)*n);
 	int ccnum=0;
 	for (int i=0;i<n;i++){
 		if (isvisited[i]=='v'){
			//printf("visit node id is %d\n",i);
 			DFS(G,n,i,ccnum);
 			ccnum++;
 		}
 	}
	memset(isvisited,'v',sizeof(char)*n);
	return ccnum;
}
int main(int argc, char *argv[]){
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
	char *outfile=new char[5000];
	strcpy(outfile,argv[1]);
	strcat(outfile,"_cc.txt");
	FILE *wfile=fopen(outfile,"w");
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
