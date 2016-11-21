#include"IOfunction.h"
#include<vector>
using namespace std;
class Randomwalk{
	Node *G;
	char *isvisit;
	int *ccid;
	int nodenum;
	vector<double> score;
	vector<double> tempvec;
	int *visitstack;
	int stacksize;
public:
	Randomwalk(){
	};
	Randomwalk(int n, Node *_G, char *_isvisit){
		nodenum=n;
		G=_G;
		isvisit=_isvisit;
	}
	void InitScore(vector<double> &_score){
		score=_score;
		tempvec.resize(score.size());
	}
	void InitStack(int *_stack, int _size){
		visitstack=_stack;
		stacksize = _size;
	}
	void InitCC(int *_cc){
		ccid=_cc;
	}
	vector<int> DFS(int v, int ccnum){
		if(visitstack==NULL){
			visitstack=(int*)malloc(sizeof(int)*nodenum);
			stacksize=nodenum;
		}
		vector<int> result;
		int pos=0;
		visitstack[pos]=v;
		pos++;
		while(pos>0){
			int u=visitstack[pos-1];
			if(isvisit[u]=='w'){
				isvisit[u]='g';
			}
			bool flag=true;
			for(int i=0;i<G[u].deg;i++){
				int nb=G[u].nbv[i];
				if(isvisit[nb]=='w'){
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
					flag=false;
				}
			}
			if(flag==true){
				if (isvisit[u] != 'b')
					result.push_back(u);
				isvisit[u] = 'b';
				ccid[u] = ccnum;
				pos--;
			}
		}
		return result;
	}
	void Normscore(vector<int> &cand){
		double max=0;
		for (int i = 0; i < (int)cand.size(); i++){
			int j = cand[i];
			max += (score[j]*score[j]);
		}
		max=sqrt(max);
		/*if (cand.size()>1)
			cout << max << endl;*/
		for (int i = 0; i < (int) cand.size(); i++){
			int j = cand[i];
			score[j]/=max;
			//cout << score[j] << endl;
		}
	}
	void RootRW(int v, int iter, vector<int> &cand){
		if (cand.size() <= 1)
			return;
		score[v]=1.0;
		int id=ccid[v];
		for(int i=0;i<iter;i++){
			copy(score.begin(), score.end(), tempvec.begin());
			for (int m = 0; m < (int) cand.size(); m++){
				int j = cand[m];
				if (j == v)
					continue;
				double temp = 0;
				double sum = 0;
				for (int k = 0; k<G[j].deg; k++){
					int w = G[j].nbv[k];
					double weight = G[j].weight[k];
					temp += tempvec[w] * weight;
					sum += weight;
				}
				if (sum != 0)
					temp /= sum;
				else
					temp = 0;
				score[j] = tempvec[j] * 0.15 + 0.85*temp;
				//score[j] = temp;
			}
			Normscore(cand);
		}
	}
	void printresult(int v){
		cout<<v;
		for(int i=0;i<nodenum;i++){
			if(ccid[i]==ccid[v]){
				cout<<":"<<i<<","<<score[i];
			}
		}
		cout<<endl;
	}
	void saveresult(int v, FILE *wfile){
		fprintf(wfile,"%d",v);
		for(int i=0;i<nodenum;i++){
			if(ccid[i]==ccid[v]){
				fprintf(wfile,":%d,%lf",i,score[i]);
			}
		}
		fprintf(wfile,"\n");
	}
	void saveresultstring(int v, FILE *wfile, vector<string> &names, vector< vector<string> > &uris,vector<int> &cand,double thres){
		fprintf(wfile, "{\"cluster-id\": \"%ld\", ", JSHash(names[v]));
		fprintf(wfile,"\"centroid_phone\": \"%s\", ",names[v].c_str());
		fprintf(wfile, "\"phones\": [");
		map<string,double> uniqueuri;
		for (auto &x : uris[v]) {
			uniqueuri[x] = 1.0;
		}
		double maxv = 1.0;
		for (int i = 0; i < (int) cand.size(); i++){
			int j = cand[i];
			if (j != v){
				if (score[j]>thres) {
					fprintf(wfile, "{\"name\": \"%s\", \"weight\": %lf}, ", names[j].c_str(), score[j]);
					for (auto &x : uris[j]) {
						double old = uniqueuri[x];
						uniqueuri[x] = (old +score[j]);
						if (uniqueuri[x] > maxv) {
							maxv = uniqueuri[x];
						}
					}
				}
			}
		}
		fprintf(wfile, "{\"name\": \"%s\", \"weight\": %lf}], ", names[v].c_str(), 1.0);
		fprintf(wfile, "\"CDRIDs\": [");
		map<string,double>::iterator iter;
		for (iter = uniqueuri.begin(); iter != uniqueuri.end(); iter++) {
			if (iter != uniqueuri.end() && (iter == --uniqueuri.end())) {
				fprintf(wfile, "{\"uri\": \"%s\", \"weight\": %lf}]", iter->first.c_str(), iter->second/maxv);
			}
			else {
				fprintf(wfile, "{\"uri\": \"%s\", \"weight\": %lf}, ", iter->first.c_str(), iter->second/maxv);
			}
		}
		fprintf(wfile,"}\n");
	}
};