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
double heuristic1(node_t currentNode, node_t destinationNode){
