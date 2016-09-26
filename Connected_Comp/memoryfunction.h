#ifndef memoryfunction_h_
#define memoryfunction_h_
#include"function.h"
/*
This file implements the memory functions which dynamically allocate memory for graphs and community matrices

/*@author: linhong (linhong.seba.zhu@gmail.com)
*/
///----------------------------------------------------------------------//
//========================================================================================================//
//functions that are related to memory management//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Initmemory(){
	/////////////////////////////////////////////////////////////
	////////////initializing memory block (START)////////////////
	////////////////////////////////////////////////////////////
	//Initialize memory blocks (START)
	curBlk = endBlk = 0;
	memBlkAr[0] = curMemPos = new char[BLK_SZ];
	curMemEnd = curMemPos + BLK_SZ;
	//Initialize memory blocks (END)
	//Initialize memory blocks (START)
	curBlk2 = 0;
	memBlkAr2[0] = curMemPos2 = new char[BLK_SZ2];
	curMemEnd2 = curMemPos2 + BLK_SZ2;
	//Initialize memory blocks (END)
	/////////////////////////////////////////////////////////
	////////////initializing memory block (end)////////////////
	////////////////////////////////////////////////////////
}
inline void InitIOmemory(){
	////////////////////////////////////////////////////////////
	///////////initializing IO memory block (START)/////////////
	///////////////////////////////////////////////////////////
	BYTE_TO_READ = 67108864;
	inputbuffer = new char[BYTE_TO_READ];
	outputbuffer = new char[BYTE_TO_READ];
	outcurpos = outputbuffer;
	outendpos = outputbuffer + BYTE_TO_READ;
	temp_string = new char[128];
	////////////////////////////////////////////////////////////
	///////////initializing IO memory block (START)/////////////
	///////////////////////////////////////////////////////////
}
inline void InitTempSubmemory(int maxnodenum){
    //Initilize the memory usage (start)
    //group id used in community initilization
    groupid = new int[maxnodenum];
    if (groupid == NULL){
        printf("could not allocate more memory\n");
        exit(2);
    }
    memset(groupid, 0, sizeof(int)*maxnodenum);
    //a temp array with integer for storing temp values
    tempidsize = new int[maxnodenum];
    if (tempidsize == NULL){
        printf("could not allocate more memory\n");
        exit(2);
    }
    memset(tempidsize, 0, sizeof(int)*maxnodenum);
    //a temp array with double for storing temp values
    tempvalue = new double[maxnodenum];
    if (tempvalue == NULL){
        printf("could not allocate more memory\n");
        exit(2);
    }
    //isseednode: a char array which used to label nodes
    isseednode =new char[maxnodenum];
    if (isseednode == NULL){
        printf("could not allocate more memory\n");
        exit(2);
    }
    memset(isseednode, 'v', sizeof(char)*maxnodenum);
    //node2subidx: an integer array which maps node id in
    //original graph to node id in sub graphs
    node2subidx = new int [maxnodenum];
    if (node2subidx == NULL){
        printf("could not allocate more memory\n");
        exit(2);
    }
    //Initilize the memory usage (end)
}
inline void releaseInitZmemory(){
    if (tempidsize != NULL){
        delete[]tempidsize;
        tempidsize = NULL;
    }
    if (groupid != NULL){
        delete[]groupid;
        groupid = NULL;
    }
}
inline void releaseTempSubmemory(){
    if (isseednode != NULL){
        delete []isseednode;
        isseednode = NULL;
    }
    if (node2subidx != NULL){
        delete []node2subidx;
        node2subidx = NULL;
    }
    if (tempvalue != NULL){
        delete []tempvalue;
        tempvalue = NULL;
    }
}
inline void InitTempFullmemory(int maxnodenum){
    //Initilize the memory usage (start)
    //group id used in community initilization
    groupid = new int[maxnodenum];
    if (groupid == NULL){
        printf("could not allocate more memory\n");
        exit(2);
    }
    memset(groupid, 0, sizeof(int)*maxnodenum);
    //a temp array with integer for storing temp values
    tempidsize = new int[maxnodenum];
    if (tempidsize == NULL){
        printf("could not allocate more memory\n");
        exit(2);
    }
    memset(tempidsize, 0, sizeof(int)*maxnodenum);
    //a temp array with double for storing temp values
    tempvalue = new double[maxnodenum];
    if (tempvalue == NULL){
        printf("could not allocate more memory\n");
        exit(2);
    }
    //Initilize the memory usage (end)

}
//check whether tmpblock can allocate a memory block with length size
void inline allocatetmpmemory(unsigned int size){
		if(size>=(size_t)(curMemEnd - curMemPos)||(curMemPos==NULL)||(curMemEnd - curMemPos)>BLK_SZ){//free mem in cur block is not enough
			if(curBlk < endBlk){
				//we have already allocated free blocks
				curMemPos=memBlkAr[++curBlk];
				curMemEnd=curMemPos+BLK_SZ;
			}else{
				//allocate a new block
				++endBlk;
				if(endBlk>=BLK_NUM){
					printf("system is unable to allocate more temporal memory\n");
					printf("number of block is %d\n",endBlk);
					exit(0);
				}
				curMemPos = new char[BLK_SZ];
				if(curMemPos==NULL){
					printf("system is unable to allocate more temporal memory\n");
					printf("number of block is %d\n",curBlk);
					printf("number of static block is %d\n",curBlk2);
					exit(2);
				}
				memBlkAr[++curBlk]=curMemPos;
				curMemEnd=curMemPos+BLK_SZ;
			}
		}//end of if free mem is not enough
	}

//check whether the pemblock can allocate a memory block with length size
void inline allocatepermemory(unsigned int size){
		if(size>=(size_t)(curMemEnd2 - curMemPos2)||(curMemPos2==NULL)||(curMemEnd2 - curMemPos2)>BLK_SZ2){//free mem in cur block is not enough
			//free mem in cur block is not enough
			//allocate a new block
			if(curBlk2<BLK_NUM2-1){
				++curBlk2;
				curMemPos2=new char[BLK_SZ2];
				if(curMemPos2==NULL){
					printf("system is unable to allocate more static memory\n");
					printf("number of block is %d\n",curBlk);
					printf("number of static block is %d\n",curBlk2);
					exit(2);
				}
				memBlkAr2[curBlk2]=curMemPos2;
				curMemEnd2=curMemPos2+BLK_SZ2;
			}else{
				printf("system is unable to allocate more static memory\n");
				exit(2);
			}
		}
}




/*
release graph memory where G is in Node structure
*/
inline void releasegraphmemory(Node *&G, int gnodenum){
	int i = 0;
	for (i = 0; i < gnodenum; i++){
		if (G[i].deg*sizeof(double) >= BLK_SZ2){
			if (G[i].nbv != NULL){
				free(G[i].nbv);
				G[i].nbv = NULL;
			}
			if (G[i].weight != NULL){
				free(G[i].weight);
				G[i].weight = NULL;
			}
		}
		G[i].vid = -1;
		G[i].deg = 0;
		G[i].nbv = NULL;
		G[i].weight = NULL;
	}
}
/*
release graph memory where G is in DNode structure
*/
inline void releasegraphmemory(DNode *&G, int gnodenum){
	int i = 0;
	for (i = 0; i < gnodenum; i++){
		if (G[i].nbv != NULL){
			free(G[i].nbv);
			G[i].nbv = NULL;
		}
		if (G[i].weight != NULL){
			free(G[i].weight);
			G[i].weight = NULL;
		}
		G[i].vid = -1;
		G[i].deg = 0;
		G[i].size = 0;
		G[i].nbv = NULL;
		G[i].weight = NULL;
	}
}

/*
release preallocated block memory
*/
inline void releaseblockmemory(){
	while (curBlk2 >= 0){
		if (memBlkAr2[curBlk2] != NULL){
			delete[] memBlkAr2[curBlk2];
			curBlk2--;
		}
		else
			curBlk2--;
	}
	curBlk = 0;
	while (curBlk <= endBlk){
		if (memBlkAr[curBlk] != NULL){
			delete[]memBlkAr[curBlk];
			curBlk++;
		}
		else
			curBlk++;
	}
}
/*
release preallocated IO memory
*/
inline void releaseIOmemory(){
	if (inputbuffer != NULL){
		delete[]inputbuffer;
		inputbuffer = NULL;
	}
	if (outputbuffer != NULL){
		delete[]outputbuffer;
		outputbuffer = NULL;
	}
    if(filebuffer!=NULL){
        delete[]filebuffer;
        filebuffer=NULL;
    }
	if (temp_string != NULL){
		delete[]temp_string;
		temp_string = NULL;
	}
}
#endif


