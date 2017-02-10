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
 *  Since we are on the
 *  Earth surface, we will use the great circle distance. In
 *  particular, we will use the haversine formula.
 *
 *  Input:
 *      n1,n2: nodes to compute the distance between.
 *
 *  Return: approximate distance.
 */
double dis2nodes(node_t n1, node_t n2){
    double lat1 = n1.lat*DEG2RAD;
    double lat2 = n2.lat*DEG2RAD;
    double lon1 = n1.lon*DEG2RAD;
    double lon2 = n2.lon*DEG2RAD;
    double dlat = lat1-lat2;
    double dlon = lon1-lon2;
    double slat = sin(dlat*0.5);
    double slon = sin(dlon*0.5);

    return 2.*asin(sqrt(POW2(slat)+
           cos(lat1)*cos(lat2)*
           POW2(slon)))*EARTH_RADIUS;
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
    double lat1 = currentNode.lat*DEG2RAD;
    double lat2 = destinationNode.lat*DEG2RAD;
    double lon1 = currentNode.lon*DEG2RAD;
    double lon2 = destinationNode.lon*DEG2RAD;
    double dlat = lat1-lat2;
    double dlon = lon1-lon2;
    double slat = sin(dlat*0.5);
    double slon = sin(dlon*0.5);

    return 2.*asin(sqrt(POW2(slat)+
           cos(lat1)*cos(lat2)*
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
uint32_t findNode(node_t *nodes, uint32_t nNodes, uint32_t targetId){
    node_t auxNode, *resultNode;
    auxNode.id = targetId;
    resultNode = bsearch(&auxNode,nodes,nNodes,sizeof(node_t),compare_id);
    if(resultNode == NULL)
        return -1;
    else
        return (resultNode-nodes);
}

/*  INSERTNODETOQUEUE
 *
 *  Puts node into queue dynamic list in a sorted way,
 *  from smallest f to biggest f, obtained from status.
 *  
 *
 *  Input:
 *      queue: queue pointer to pointer of the first element.
 *      nodeId: ID to put into list in a sorted way.
 *      status: vector of status of the nodes in the algorithm.
 */
void insertNodeToQueue(queue_t **queue,uint32_t nodeId,
                       AStarStatus_t *status){
    queue_t *auxQueue,*queueIterator; 
    auxQueue = malloc(sizeof(queue_t)); assert(auxQueue);
    auxQueue->id = nodeId;
    //Insert at the begining
    if(status[(*queue)->id].f >= status[nodeId].f){
        auxQueue->next = *queue;
        *queue = auxQueue;
    //Insert somewhere else
    }else{
        queueIterator = *queue;
        while(queueIterator->next != NULL && 
              status[queueIterator->next->id].f < status[nodeId].f)
            queueIterator = queueIterator->next;
        auxQueue->next = queueIterator->next;
        queueIterator->next = auxQueue;
    }

}

/*  DELETENODEFROMQUEUE
 *
 *  Deletes dynamic list element of queue containing nodeID.
 *  User must be sure that the ID is in the list. 
 *
 *  Input:
 *      queue: queue pointer to pointer of the first element.
 *      nodeId: ID to put into list in a sorted way.
 */
void deleteNodefromQueue(queue_t **queue, uint32_t nodeId){
    queue_t *auxQueue,*queueIterator;
    //Delete first element
    if((*queue)->id == nodeId){
        auxQueue = *queue;
        *queue = auxQueue->next;
        free(auxQueue);
    //Delete other element
    }else{
        queueIterator = *queue;
        while(queueIterator->next->id != nodeId)
            queueIterator = queueIterator->next;
        auxQueue = queueIterator->next;
        queueIterator->next = auxQueue->next;
        free(auxQueue);
    }
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
                    uint32_t nNodes, uint32_t startNode,
                    uint32_t targetNode){
    queue_t *open,*auxQueue;
    uint32_t currentNode,successorNode;
    uint8_t i;
    double successorCurrentCost;
    
    /* Initialize */
    open = malloc(sizeof(queue_t)); assert(open);
    open->next = NULL;
    open->id = startNode;

    status[startNode].g = 0.;
    status[startNode].h = heuristic1(nodes[startNode],nodes[targetNode]);
    status[startNode].f = status[startNode].g+status[startNode].h;
    status[startNode].whq = OPEN;

    /* Main Loop */
    while(open != NULL){
        // Select current node with smallest f
        currentNode = open->id;
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
                else
                    deleteNodefromQueue(&open,successorNode);
            }else if(status[successorNode].whq == CLOSED){
                if(status[successorNode].g <= successorCurrentCost)
                    continue;
                //Add successor node to open list
                status[successorNode].whq = OPEN;
            }else{
                //Add successor node to open list
                status[successorNode].whq = OPEN;
                status[successorNode].h = heuristic1(nodes[successorNode],nodes[targetNode]);
            }
            status[successorNode].g = successorCurrentCost;
            status[successorNode].f = status[successorNode].g + status[successorNode].h;
            status[successorNode].parent = currentNode;
            insertNodeToQueue(&open,successorNode,status);
        }
        //Add current node to CLOSED list (also remove from open)
        status[currentNode].whq = CLOSED;
        deleteNodefromQueue(&open,currentNode);
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
