#include "mkGr.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    
    char *line;
    uint8_t i,commLin;
    uint32_t nNodes, nWays,j,nSucc,nameLen,maxChar;
    node_t *nodes;
    FILE *input, *binOut;
    
    /* INPUT */
    if (argc < 8
        || sscanf(argv[4],"%"SCNi8, &commLin)!=1
        || sscanf(argv[5],"%"SCNi32, &nNodes)!=1
        || sscanf(argv[6],"%"SCNi32, &nWays)!=1
        || sscanf(argv[7],"%"SCNi32, &maxChar)!=1
       ) {
          fprintf(stderr,"%s filename outputname delim commentedLines numNodes numWays maxChar\n",argv[0]);
          return 1;
    }

    line = malloc(sizeof(char)*maxChar+1); assert(line);
    nodes = malloc(sizeof(node_t)*nNodes); assert(nodes);

    input = fopen(argv[1],"r");

    for(i=0; i<commLin; i++)
        fgets(line,maxChar,input);

    for(j=0; j<nNodes; j++){
        fgets(line,maxChar,input);
        nodes[j] = load_node(line,argv[3]);
    }
    
    for(j=0; j<nWays; j++){
        fgets(line,maxChar,input);
        add_way(nodes,nNodes,line,argv[3]);
    }
    
    fclose(input);

    /* WRITE GRAPH INTO BINARY FILE */
    nSucc = nameLen = 0;
    //Compute total successors and name lenghts
    for(j=0; j<nNodes; j++){
        nSucc += nodes[j].nsucc;
        nameLen += strlen(nodes[j].name)+1;
    }
    
    //Write first 
    binOut = fopen(argv[2],"wb");
    fwrite(&nNodes,sizeof(uint32_t),1,binOut);
    fwrite(&nSucc,sizeof(uint32_t),1,binOut);
    fwrite(&nameLen,sizeof(uint32_t),1,binOut);

    fwrite(nodes,sizeof(node_t),nNodes,binOut);
    
    for(j=0; j<nNodes; j++){
        if(nodes[j].nsucc != 0)
            fwrite(nodes[j].successors,sizeof(uint32_t),nodes[j].nsucc,binOut);
        free(nodes[j].successors);
    }
    
    for(j=0; j<nNodes; j++){
        fwrite(nodes[j].name,sizeof(char),strlen(nodes[j].name),binOut);
        fwrite("\0",sizeof(char),1,binOut);
        free(nodes[j].name);
    }

    /* FREE MEMORY */
    fclose(binOut);
    
    free(nodes); free(line);
    
    return 0;
}
