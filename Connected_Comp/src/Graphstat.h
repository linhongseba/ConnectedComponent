#include"IOfunction.h"
/*
A simple implementation for the connected componennt algorithm
@author: Linhong Zhu (linhong.seba.zhu@gmail.com)
@last_update_time: April 14, 2016
@bug report: please create issues in GitHub.
copyright 1.0 Linhong Zhu
under Apache 2.0 licence
*/
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