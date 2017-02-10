#include "aStar.h"
#include "mkGr.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char *argv[]){
    
    uint32_t nNodes,nSucc,nameLen; //Number of nodes, succesors, and total name lenght
    uint32_t startId, targetId, startNode, targetNode; //nodeIDs and vectorIDs
    uint32_t aux1,aux2,i;
    uint32_t *successors; //Successors vector
    char *nodeNames;    //Nodenamevector
    AStarStatus_t *status; //A star status vector for all nodes
    node_t *nodes; //Node vector
    FILE *binIn, *solutionF; //Binary input file and solution file
    struct timeval tval_before, tval_after, tval_result; //Timing
    
    /* INPUT */
    if (argc < 4 ||
        sscanf(argv[2],"%"SCNi32, &startId)!=1 ||
        sscanf(argv[3],"%"SCNi32, &targetId)!=1 
       ) {
          fprintf(stderr,"%s filename startId targetId\n",argv[0]);
          return 1;
    }

    /* READ GRAPH FROM BINARY FILE */
    binIn = fopen(argv[1],"r");
    //Read header variables
    if((fread(&nNodes,sizeof(uint32_t),1,binIn) +
        fread(&nSucc,sizeof(uint32_t),1,binIn)+
        fread(&nameLen,sizeof(uint32_t),1,binIn)) != 3){
            fprintf(stderr,"Problems reading header. Exiting...\n");
            fclose(binIn);
            return 1;
    }

    //Alloc memory
    nodes = malloc(sizeof(node_t)*nNodes); assert(nodes);
    successors = malloc(sizeof(uint32_t)*nSucc); assert(successors);
    nodeNames = malloc(sizeof(char)*nameLen); assert(nodeNames);

    //Read nodes, successors and nodeNames together
    if((fread(nodes,sizeof(node_t),nNodes,binIn) +
        fread(successors,sizeof(uint32_t),nSucc,binIn) +
        fread(nodeNames,sizeof(char),nameLen,binIn)) != (nNodes+nSucc+nameLen)){
            fprintf(stderr,"Problems reading graph data. Exiting...\n");
            free(nodes);
            free(successors);
            free(nodeNames);
            fclose(binIn);
            return 1;
    }

    fclose(binIn);
    
    //Put node name and successors into each node
    aux1 = aux2 = 0;
    for(i=0;i<nNodes;i++){
        if(nodes[i].nsucc != 0){
            nodes[i].successors = successors+aux1;
            aux1 += nodes[i].nsucc;
        }
        nodes[i].name = nodeNames+aux2;
        aux2 += 1+strlen(nodeNames+aux2);
    }
    
    /* Find initial and target nodes */
    startNode = findNode(nodes,nNodes,startId);
    if(startNode == -1){
        fprintf(stderr,"ERROR: Start node not found in graph.\n");
        return -1;
    }else
        fprintf(stderr,"Starting node found in position %"PRIu32".\n",startNode);
    targetNode = findNode(nodes,nNodes,targetId);
    if(targetNode == -1){
        fprintf(stderr,"ERROR: Target node not found in graph.\n");
        return -2;
    }else
        fprintf(stderr,"Target node found in position %"PRIu32".\n",targetNode);
    /* Initiate status */
    status = malloc(sizeof(AStarStatus_t)*nNodes); assert(status);
    for(i=0; i<nNodes;i++)
        status[i].whq = NONE;

    /* A-star algorithm */
    gettimeofday(&tval_before,NULL);
    if(aStarAlgorithm(nodes,status,nNodes,startNode,targetNode) == 0){
        fprintf(stderr,"Solution found, with distance %lf\n",status[targetNode].g);
    }else{
        fprintf(stderr,"ERROR: No path was found\n");
    }
    gettimeofday(&tval_after,NULL);
    timersub(&tval_after,&tval_before,&tval_result);
    fprintf(stdout,"Time of algorithm: %2ld.%06ld\n",
            (long int)tval_result.tv_sec,(long int)tval_result.tv_usec);

    //Print solution
    solutionF = fopen("solution.dat","w");
    if(solutionF != NULL){
        aux1 = targetNode;
        while(aux1 != startNode){
            fprintf(solutionF,"Node id: %10"PRIu32" | Distance: %10.2lf | Name: %s\n",
                    nodes[aux1].id,status[aux1].g,nodes[aux1].name);
            aux1 = status[aux1].parent;
        }
        fprintf(solutionF,"Node id: %10"PRIu32" | Distance: %10.2lf | Name: %s\n",
                nodes[aux1].id,status[aux1].g,nodes[aux1].name);
        fclose(solutionF);
    }else{
        fprintf(stderr,"Could not create solution file\n");
    }

    //Free memory
    free(nodes); free(successors); free(nodeNames); free(status);
    
    return 0;
}
