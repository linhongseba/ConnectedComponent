#include"IOfunction.h"
#include"randomwalk.h"
int main(int argc, char *argv[]){
    if(argc<4){
        printf("Usage: %s [input_graph_file] [phone_name_file] [phone_uri_file][output_file] [threshold]\n", argv[0]);
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
    ReadWeightGraph(G, n, rfile);
    fclose(rfile);
    //PrintGraph(G,n);
    //read the new graph (end)//
    int *ccid=new int[n];
    memset(ccid,0,sizeof(int)*n);
    char *isvisited=new char[n];
    memset(isvisited, 'w', sizeof(char)*n);
    int *visitstack=(int*)malloc(sizeof(int)*n);
    vector<double> scores(n,0.0);
    vector<string> names(n,"");
    vector < vector<string> > uris(n, vector<string>());
    std::ifstream fin(argv[2]);
    Readnames(fin, names);
    std::ifstream fin2(argv[3]);
    Readuris(fin2, uris);
    /*for (int i = 0; i < n; i++) {
        vector<string> res = uris[i];
        for (auto & x : res) {
            cout << x << "\t";
        }
        cout << endl;
    }*/
    Randomwalk RW(n,G,isvisited);
    RW.InitCC(ccid);
    RW.InitStack(visitstack,n);
    RW.InitScore(scores);
    int ccnum=1;
    FILE *wfile=fopen(argv[4],"w");
    if(wfile==NULL){
        printf("could not open file to write\n");
        exit(3);
    }
    double thres = atof(argv[5]);
    for (int i = 0; i < n; i ++) {
        if (isvisited[i] == 'w') {
             vector<int> cluster = RW.DFS(i, ccnum);
            ccnum ++;
            if (cluster.size() == 1) {
                RW.setScore(i);
                RW.saveresultstring(i, wfile,names, uris, cluster,thres);
            } else  if (cluster.size() < 1000){
                for (int j = 0; j < (int)cluster.size(); j++) {
                    RW.RootRW(cluster[j], 100, cluster);
                    RW.saveresultstring(cluster[j], wfile, names, uris,cluster, thres);
                }
            } else {
                cout<<"large cluster  " << cluster.size()<<endl;
                for (int j = 0; j < (int)cluster.size(); j++) {
                    int nid =  cluster[j] ;
                    if (G[nid].deg  > 100) {
                        RW.RootRW(nid, 50, cluster);
                        RW.saveresultstring(nid, wfile, names, uris, cluster, thres);
                    } else {
                         RW.RootRW(nid, 10, cluster);
                         RW.saveresultstring(nid, wfile,names, uris, cluster,thres);
                    }
                    if (j%1000 == 0) {
                        cout<<"in computing " << j <<endl;
                    }
                } // end of for scanning cluster 
                cout<<"finish computing the large cluster" <<endl;
            } // end of else  cluster.size >= 1000
        } // end of unvisited scanning
    } // end of for
    if(wfile!=NULL)
        fclose(wfile);
    printf("total number of connected component is %d\n",ccnum-1);
    if(isvisited!=NULL)
        delete[]isvisited;
    //if(visitstack!=NULL)
        //free(visitstack);
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
