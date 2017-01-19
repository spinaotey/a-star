#pragma once
#define EARTH_RADIUS 6371008.8 //mean earth radius (meters)
#include <inttypes.h>

typedef uint8_t Queue;
enum whichQueue {NONE, OPEN, CLOSED};

typedef struct AStarStatus_s{
    double g,h,f;
    uint64_t parent;
    Queue whq;
} AStarStatus_t;

typedef struct queue_s{
    struct queue_s *next;
    uint64_t id;
} queue_t;

typedef struct node_s node_t;

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
double dis2nodes(node_t n1, node_t n2);

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
double heuristic1(node_t currentNode, node_t destinationNode);

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
uint64_t findNode(node_t *nodes, uint64_t nNodes, uint64_t targetId);

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
                    uint64_t targetNode);
