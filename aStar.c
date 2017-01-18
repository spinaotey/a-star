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
