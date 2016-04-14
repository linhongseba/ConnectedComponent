#ifndef IOfunction_h_
#define IOfunction_h_
#include"function.h"
#include"memoryfunction.h"
/*
This file implements the IO functions which read/write graph files

/*@author: linhong (linhong.seba.zhu@gmail.com)
@bug report: please create issues in GitHub
@ last update: July, 2015

*/
///----------------------------------------------------------------------//

int listfilename(vector<char*> &filenames, char *dirname){
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(dirname)) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_name[0] != '.'){
				//printf("%s\n", ent->d_name);
				allocatetmpmemory(sizeof(char) * 500);
				char *name = (char*)curMemPos;
				curMemPos += (sizeof(char) * 500);
				strcpy(name, dirname);
				strcat(name, "/");
				strcat(name, ent->d_name);
				//printf("%s\n", name);
				filenames.push_back(name);
			}
		}
		closedir(dir);
		return 0;
	}
	else {
		/* could not open directory */
		perror("");
		return EXIT_FAILURE;
	}
}
//==================================================================================
//////////////////////////////////////////////////////////////////////////////////
//Each time we read a fixed number of bits into input file buffer (start)
//=================================================================================
inline void GetData(char *dest, int byte2read, FILE *inFile){
	if (!feof(inFile)){
		byteread = fread(dest, 1, byte2read, inFile);
		//		printf("inputbufferB[0] = %d, inputbufferB[1] = %d, inputbufferB[2] = %d, inputbufferB[3] = %d, inputbufferB[4] = %d, inputbufferB[5] = %d \n", inputbufferB[0],inputbufferB[1],inputbufferB[2],  inputbufferB[3],inputbufferB[4],inputbufferB[5]);
		//		exit(1);
		if (byteread < byte2read){
			//end of file or more to read
			if (!feof(inFile)){
				//Tests if a error has occurred in the last reading or writing operation with the given stream,
				//returning a non-zero value in case of error.
				if (ferror(inFile)){
					perror("Error reading from input file. Abort!\n");
					exit(1);
				}
			}
		}
	}
}
//==================================================================================
//////////////////////////////////////////////////////////////////////////////////
//Each time we read a fixed number of bits into input file buffer (end)
//=================================================================================
//===============================================================================
//////////////////////////////////////////////////////////////////////////////////
//Read data into input file buffer (start)
//================================================================================
inline void FillInputBuffer(FILE *inFile){
	int i=0;
	endpos=endpos0;
	if(endpos > curpos){
		i=endpos-curpos;
		//this essentially moves data after curpos to the start of the buffer
		//and then call GetData to fill data to the position starting from endpos
		memmove(inputbuffer,curpos,i);
		endpos=inputbuffer+i;
		//get data from Input file
		GetData(endpos,BYTE_TO_READ-i,inFile);	//new end position
	}
	else{	
		endpos=inputbuffer;
		//get data from Input file
		GetData(endpos,BYTE_TO_READ,inFile);	//new end position
	}

	endpos+=byteread;

	if(!feof(inFile)){
		//we seek back to the end of the last complete line
		endpos0=endpos--;

		while(*endpos != '\n'){
			--endpos;
		}
		++endpos;	//endpos is one pos after '\n'
	}

	curpos=inputbuffer;
}
//===============================================================================
//////////////////////////////////////////////////////////////////////////////////
//Read data into input file buffer (end)
//================================================================================




//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//parse the value of vertex id from char * type input buffer
//===================================================================================
inline void Parseid(int &v){
	v=0;
	do{
		v=(10 * v)+int(*curpos)-48;
	}while(*(++curpos) != ',');
}

//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//Skip lines
//===================================================================================
inline void Skipline(){
	while(*curpos!='\n')
		curpos++;
}

//================================================================================
//parse the value of node degree from char *type input buffer
//================================================================================
inline void Parsedeg(int &dv, int nodenum){
	dv=0;
	if(*(++curpos) != '0'){
		dv=0;
		do{
			dv=(10 * dv)+int(*curpos)-48;
		}while(*(++curpos) != ':');
		if(dv<0||dv>nodenum){
			printf("error in graph degree\n");
			exit(1);
		}
	}
}

//==============================================================================
//parse the value of neighbore id and its weight from char *type input buffer
//==============================================================================
inline void Parseneighbor(int &neighbore,int nodenum, int v){
	++curpos;
	neighbore = 0;
	while((*curpos)>='0'&&(*curpos)<='9'){
		neighbore=(10 * neighbore)+int(*curpos)-48;
		curpos++;
	}
	if(neighbore<0||neighbore>=nodenum){
		printf("error,please check node %d and neighbore %d\n",v, neighbore);
		exit(1);
	}
}

inline void Replacefile(char *destfilename, char *srcfilename){
	FILE* source = fopen(srcfilename, "rb");
	FILE* dest = fopen(destfilename, "wb");
	if(source!=NULL&&dest!=NULL){
		size_t size;
		// clean and more secure
		// feof(FILE* stream) returns non-zero if the end of file indicator for stream is set

		while (size = fread(filebuffer, 1, BYTE_TO_READ, source)) {
			fwrite(filebuffer, 1, size, dest);
		}

		fclose(source);
		fclose(dest);
	}
}
inline void ReadGraph(Node *&G, int nodenum,FILE *inFile){
	int i=0;
	int v=0;
	int dv=0;
	int neighbore=0;
	int j=0;
	///////////////////////////////////////////////////////////////////////////////////////////////
	//////////scanning the graph again to initializing the S and Splus nodes' adjacent lists(start)
	//////////////////////////////////////////////////////////////////////////////////////////////
	G=(Node*)malloc(sizeof(Node)*nodenum);
	if(G==NULL){
		printf("system could not allocate more memory\n");
		exit(2);
	}
	for(i=0;i<nodenum;i++){
		G[i].vid=-1;
		G[i].deg=0;
		G[i].nbv=NULL;
		G[i].weight=NULL;
	}
	curpos=endpos0=inputbuffer;	//initialize the data position pointers to the start of the buffer
	FillInputBuffer(inFile);
	//starting scanning
	int pos=0;
	while(1){
		Parseid(v);
		//skip the lines where the format of graph has some problem
		if(v<0||v>=nodenum){
			Skipline();
			if(++curpos >= endpos){
				if(feof(inFile))
					break;
				FillInputBuffer(inFile);
			}
			continue;
		}//end of skip "bad" lines
		if (pos >= nodenum)
			break;
		G[pos].vid=v;
		dv=0;
		Parsedeg(dv,nodenum);
		if(dv>0){
			/////////////////////////////////////////////////////////////
			//for each node v,
			//allocate memory space to store a heap entry (dv,v,nb(v)) (START)
			///////////////////////////////////////////////////////////
			if(sizeof(double)*dv>=BLK_SZ2){
				G[pos].nbv=(int*)malloc(sizeof(int)*dv);
				G[pos].weight=NULL;
				if (G[pos].nbv==NULL){
					printf("System could not allocate more memory\n");
					exit(2);
				}
			}else{
				allocatetmpmemory(sizeof(int)*dv);
				G[pos].nbv=(int *)curMemPos;
				curMemPos+=(sizeof(int)*dv);
			}

			/////////////////////////////////////////////////////////////
			//for each node v,
			//allocate memory space to store a heap entry (dv,v,nb(v)) (END)
			///////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////////////
			//read and initialize the value of neighbore index and weight (START)
			//////////////////////////////////////////////////////////////////////////
			j=0;
			G[pos].deg=0;
			while(j<dv){
				Parseneighbor(neighbore,nodenum,v);
				G[pos].nbv[G[pos].deg]=neighbore;
				G[pos].deg++;
				j++;
			}
			/////////////////////////////////////////////////////////////////////////
			//read and initialize the value of neighbore index and weight (END)
			//////////////////////////////////////////////////////////////////////////
			while(*curpos!='\n')
				curpos++;
		}//end: if(deg >0)
		else{
			G[pos].deg=0;
			G[pos].nbv=NULL;
			G[pos].weight=NULL;
			while(*curpos!='\n')
				curpos++;
		}
		pos++;
		if(++curpos >= endpos){
			if(feof(inFile))
				break;
			FillInputBuffer(inFile);

		}
	}//end: while(i)
	///////////////////////////////////////////////////////////////////////////////////////////////
	//////////scanning the graph again to initializing the S and Splus nodes' adjacent lists(end)
	//////////////////////////////////////////////////////////////////////////////////////////////
	rewind(inFile);
	int rv=fscanf(inFile,"%d\n",&nodenum);
	if (rv != 1){
		printf("the format of file is not correct");
		exit(2);

	}
}
inline void PrintGraph(Node *G, int n){
	for(int i=0;i<n;i++){
		printf("%d\t%d",i,G[i].deg);
		for(int j=0;j<G[i].deg;j++){
			printf(":%d",G[i].nbv[j]);
		}
		printf("\n");
	}
}
#endif
