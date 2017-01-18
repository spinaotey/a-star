#include "aStar.h"
#include "mkGr.h"
#include "myFunctions.h"
#include <math.h>

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
    double dlat = ABS(n1.lat-n2.lat);
    double dlon = ABS(n1.lon-n2.lon);
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
