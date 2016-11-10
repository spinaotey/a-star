#include "mkGr.h"
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



/*  strtok_single

    strtok manual implementation where zero string elements are
    taken into account.

    Variables:
        -str = Input string to find next delims.
        -delims = string to separate the input.
    
    Return value:
        Pointer to next start of word, modifiying the delims
        character by '\0' character.
 */
char * strtok_single (char * str, char const * delims){

    static char  * src = NULL;
    char  *  p,  * ret = 0;

    if (str != NULL)
        src = str;

    if (src == NULL)
        return NULL;

    if ((p = strpbrk (src, delims)) != NULL) {
        *p  = 0;
        ret = src;
        src = ++p;

    } else if (*src) {
        ret = src;
        src = NULL;
    }

    return ret;
}


/*  sep_line

    Given an input string (line) and a string of characters for separation
    (separatos), it separates the line and puts each string in a vector
    of strings (elements). It also computes the number of elements (n).

    Important: The line is useless after this function, so make sure to
    first make a copy if needed.

    Variables:
        -line = Input string to be separated.
        -separators = Characters to separate the string.
        -elements = Output vector of strings.
        -n = number of elements obtained.
 
 */
void sep_line(char *line, char *separators, char ***elements, uint32_t *n){
    
    char *token;

    *n = 0;
    *elements = (char **) malloc(sizeof(char *)); assert(*elements);

    token = strtok_single(line,separators);

    while(token != NULL){
        (*n)++;
        *elements = (char **) realloc(*elements,sizeof(char *)*(*n));assert(*elements);
        (*elements)[(*n)-1]  = (char *) malloc(sizeof(char)*strlen(token)+1);assert((*elements)[(*n)-1]);
        strcpy((*elements)[(*n)-1],token);
        token = strtok_single(NULL,separators);
    }

}




/*  load_node

    Loads node data from line into node structure. The following
    order in the elements of the line is assumed:
        1.  Node ID.
        2.  Node name.
        9.  Node latitude.
        10. Node longitude.

    Variables:
        -line = Input line with node data.
        -separator = string to separate the input.
    
    Return value:
        node filled with the data of the line
 */
node_t load_node(char *line, char *separator){
    char **elements;
    uint32_t i,n;
    node_t node;

    sep_line(line,separator, &elements,&n);
    
    sscanf(elements[1],"%"SCNi64,&node.id); // node ID

    node.name = (char *) malloc(strlen(elements[2])+1);
    strcpy(node.name,elements[2]); // node name (if available)
    
    sscanf(elements[9],"%lf",&node.lat); //node latitude
    sscanf(elements[10],"%lf",&node.lon); //node longitude

    node.nsucc = 0; //initiate number of successors

    node.successors = (uint64_t *) malloc(sizeof(uint64_t)*2);assert(node.successors);

    for(i=0; i<n; i++)
        free(elements[i]);

    free(elements);

    return node;
} 

/*  compare_id

    Compares the id's of two nodes. It is implemented to use bsearch
    from <stdlib.h>. 

    Variables:
        -node1 = pointer to first node.
        -node2 = pointer to second node.
    
    Return value:
        -1 if id_1 < id_2
        0  if id_1 == id_2
        1  if id_1 > id_2
 */
 
int compare_id(const void *node1, const void *node2){
    uint64_t id_1 = ((node_t *) node1)->id;
    uint64_t id_2 = ((node_t *) node2)->id;
    return id_1 < id_2 ? -1: (id_1 > id_2 ? 1 : 0);
}



/*  add_succ

    Adds a successor to a node, changing the size of the
    vector if necessary
 
    Variables:
        -node = node to add the successor.
        -vPos = position in the node vector.
 */
void add_succ(node_t *node, uint64_t vPos){
    uint8_t i;

    /* To not add duplicates */
    for(i=0; i< node->nsucc; i++)
        if((node->successors)[i] == vPos)
            break;

    if(i == node->nsucc){
        /* If needed, modify vector size of successors */    
        if((((node->nsucc)%2) == 0) && (node->nsucc != 0)){
            node->successors = realloc(node->successors,
                                       sizeof(uint64_t)*((node->nsucc)+2));
            assert(node->successors);
        }
        /* Adds the vector position */
        (node->successors)[node->nsucc] = vPos;
        node->nsucc++;
    }
}


/*  add_way

    Computes for a way the nodes appearing in it, their position in the node
    vector, and depending of oneway or twoway, adds the edges to the nodes.
    
    Variables:
        -nodes = vector of nodes.
        -nnodes = number of nodes.
        -line = string of the way line.
        -separator = string to delimite the columns of the line.
 */
void add_way(node_t *nodes, uint64_t nNodes, char *line, char *separator){
    uint32_t n, i, nnInWay = 0;
    uint64_t *nPos;

    
    node_t auxNode, *p;

    char **elements;
    
    sep_line(line,separator, &elements,&n);
    
    /* There has to be more than one node in the way */
    if(n > 10){
        nPos = (uint64_t *) malloc(sizeof(uint64_t)*(n-9));assert(nPos);
        /* Find node positions in the node vector from their id's */
        for(i=9; i<n; i++){
            sscanf(elements[i],"%"SCNi64,&auxNode.id);
            p = (node_t *) bsearch(&auxNode, nodes, nNodes,sizeof(node_t),compare_id);
            /* If found in our nodes */
            if(p != NULL){
                nPos[nnInWay] = (p-nodes);
                nnInWay++;
            }
        }

        /* There are enough nodes belonging to our graph */
        if(nnInWay >= 2){
            /* Oneway way */
            if(strcmp("oneway",elements[7]) == 0)
                for(i=0; i<(nnInWay-1);i++)
                    add_succ(&nodes[nPos[i]],nPos[i+1]);
            /* Twoway way */
            else{
                for(i=0; i<(nnInWay-1);i++){
                    add_succ(&nodes[nPos[i]],nPos[i+1]);
                    add_succ(&nodes[nPos[i+1]],nPos[i]);
                }
            };


        }

    }

    for(i=0; i<n; i++)
        free(elements[i]);

    free(elements);
}
