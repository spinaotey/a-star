#pragma once
#define EARTH_RADIUS 6371008.8 //mean earth radius (meters)
#include <inttypes.h>

typedef uint8_t Queue;
enum whichQueue {NONE, OPEN, CLOSED};

typedef struct AStarStatus_s{
    double g,h;
    uint64_t parent;
    Queue whq;
} AStarStatus_t;

typedef struct queue_s{
    struct queue_s *previous;
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
 *  Heuristic function invented by me for the a* algorithm.
 *  It takes a geodesic "in diagonal" and then goes straight.
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
