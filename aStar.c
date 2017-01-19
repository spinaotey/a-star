#include "aStar.h"
#include "mkGr.h"
#include "myFunctions.h"
#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*  DIS2NODES
 *
 *  Computes the distance between two nodes of the graph. We
 *  assume that two nodes are close enough to make a planar
 *  approximation of the earth.
 *
 *  Input:
 *      n1,n2: nodes to compute the distance between.
 *
 *  Return: approximate distance.
 */
double dis2nodes(node_t n1, node_t n2){
    double dlat = ABS(n1.lat-n2.lat);
    double dlon = ABS(n1.lon-n2.lon);
    return sqrt(POW2(dlat)+POW2(dlon))*EARTH_RADIUS;
}


/*  HEURISTIC1
 *
 *  Heuristic function for the a* algorithm. Since we are on the
 *  Earth surface, we will use the great circle distance. In
 *  particular, we will use the haversine formula.
 *
 *  Input:
 *      currentNode: current node to find heuristic distance.
 *      destinationNode: destination node.
 *
 *  Return: heuristic distance.
 */
double heuristic1(node_t currentNode, node_t destinationNode){
    double dlat = ABS(destinationNode.lat-currentNode.lat);
    double dlon = ABS(destinationNode.lon-currentNode.lon);
    double slat = sin(dlat*0.5);
    double slon = sin(dlon*0.5);

    return 2.*asin(sqrt(POW2(slat)+
           cos(destinationNode.lat)*cos(currentNode.lat)*
           POW2(slon)))*EARTH_RADIUS;
}

/*  FINDNODE
 *
 *  Finds node position in vector given its ID. If it is not
 *  found it returns -1.
 *
 *  Input:
 *      nodes: vector of nodes.
 *      nNodes: number of nodes in vector.
 *      targetId: ID to look for.
 *
 *  Return: position of the node with input ID. If not found it
 *          returns -1.
 */
uint64_t findNode(node_t *nodes, uint64_t nNodes, uint64_t targetId){
    node_t auxNode, *resultNode;
    auxNode.id = targetId;
    resultNode = bsearch(&auxNode,nodes,nNodes,sizeof(node_t),compare_id);
    if(resultNode == NULL)
        return -1;
    else
        return (nodes-resultNode);
}

/*  ASTARALGORITHM
 *
 *  Given a startin node and a target node in the graph, the a-star
 *  algorithm is applied to find a path between them, trying to make
 *  it as short as possible. The path can be reconstructed from the
 *  AStarStatus vector after its completation. 
 *
 *  Input:
 *      nodes: vector of nodes.
 *      status: vector of AStarStatus which will be modified.
 *      nNodes: number of nodes in vector.
 *      startNode: position of starting node in the vector of nodes.
 *      targetNode: position of target node in the vector of nodes.
 *
 *  Return: 0 if algorithm was successfull in finding a path, 1 otherwise.
 */
uint8_t aStarAlgorithm(node_t *nodes, AStarStatus_t *status, 
                    uint64_t nNodes, uint64_t startNode,
                    uint64_t targetNode){
    queue_t *open,*close,*queueIterator,*auxQueue;
    uint64_t nResult;
    uint64_t fmin,currentNode,successorNode;
    uint8_t i;
    double successorCurrentCost;
   /* 
    startNode = findNode(nodes,nNodes,startId);
    if(startNode == -1){
        fprintf(stderr,"ERROR: Start node not found in graph.\n");
        return -1;
    }
    targetNode = findNode(nodes,nNodes,targetId);
    if(targetNode == -1){
        fprintf(stderr,"ERROR: Target node not found in graph.\n");
        return -2;
    }
    */
    /* Initialize */
    open = malloc(sizeof(queue_t)); assert(open);
    open->next = NULL;
    open->id = startNode;
    
    status[startNode].parent = -1;
    status[startNode].g = 0.;
    status[startNode].h = heuristic1(nodes[startNode],nodes[targetNode]);
    status[startNode].f = status[startNode].g+status[startNode].h;
    status[startNode].whq = OPEN;

    /* Main Loop */
    while(open != NULL){
        // Select current node with smallest f
        queueIterator = open;
        fmin = status[queueIterator->id].f;
        currentNode = queueIterator->id;
        while(queueIterator->next != NULL){
            queueIterator = queueIterator->next;
            if(status[queueIterator->id].f < fmin){
                fmin = status[queueIterator->id].f;
                currentNode = queueIterator->id;
            }
        }
        // If current node is target node, we are done
        if(currentNode == targetNode)
            break;
        //Expand each successor of current node
        for(i=0; i<nodes[currentNode].nsucc;i++){
            successorNode = nodes[currentNode].successors[i];
            successorCurrentCost = status[currentNode].g + 
                                   dis2nodes(nodes[successorNode],nodes[currentNode]);
            if(status[successorNode].whq == OPEN){
                if(status[successorNode].g <= successorCurrentCost)
                    continue;
            }else if(status[successorNode].whq == CLOSED){
                if(status[successorNode].g <= successorCurrentCost)
                    continue;
                //Add successor node to open list
                status[successorNode].whq = OPEN;
                queueIterator = open;
                while(queueIterator->next != NULL)
                    queueIterator = queueIterator->next;
                queueIterator->next = malloc(sizeof(queue_t));
                queueIterator = queueIterator->next;
                queueIterator->next = NULL;
                queueIterator->id = successorNode;
            }else{
                //Add successor node to open list
                status[successorNode].whq = OPEN;
                queueIterator = open;
                while(queueIterator->next != NULL)
                    queueIterator = queueIterator->next;
                queueIterator->next = malloc(sizeof(queue_t));
                queueIterator = queueIterator->next;
                queueIterator->next = NULL;
                queueIterator->id = successorNode;
                status[successorNode].h = heuristic1(nodes[successorNode],nodes[targetNode]);
            }
            status[successorNode].g = successorCurrentCost;
            status[successorNode].f = status[successorNode].g + status[successorNode].h;
            status[successorNode].parent = currentNode;
        }
        //Add current node to CLOSED list (also remove from open)
        status[currentNode].whq = CLOSED;
        if(open->id == currentNode){
            auxQueue = open;
            open = open->next;
            free(auxQueue);
        }else{
            queueIterator = open;
            while(queueIterator->next->id != currentNode)
                queueIterator = queueIterator->next;
            auxQueue = queueIterator->next;
            queueIterator->next = auxQueue->next;
            free(auxQueue);
        }
    }
    //Free open list
    while(open != NULL){
        auxQueue = open;
        open = open->next;
        free(auxQueue);
    }
    //Return that it was found or not
    if(currentNode == targetNode)
        return 0;
    else
        return 1;
}
