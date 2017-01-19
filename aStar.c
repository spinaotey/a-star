#include "aStar.h"
#include "mkGr.h"
#include "myFunctions.h"
#include <math.h>
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
 *  Heuristic function invented by me for the a* algorithm.
 *  It takes a geodesic "in diagonal" and then goes straight.
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
    double min,max;
    if(dlat>dlon){
        max = dlat;
        min = dlon;
    }else{
        max = dlon;
        min = dlat;
    }
    return ((max-min)+2.*asin(sqrt(1.+cos(min))*sin(min*0.5)))*EARTH_RADIUS;
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

void aStarAlgorithm(node_t *nodes, AStarStatus_t *status, 
                    uint64_t nNodes, uint64_t startId,
                    uint64_t endId, uint64_t *result, 
                    uint64_t *nResult){
    
}
