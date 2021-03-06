#pragma once
#include <inttypes.h>

/* Node Structure of the graph */
typedef struct node_s{
    uint32_t id;            // Identification number
    char *name;             // Name (if available)
    double lat, lon;        // Spherical coordinates
    uint8_t nsucc;          // Number of successors
    uint32_t *successors;   // Position in node vector
} node_t;


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
char * strtok_single (char * str, char const * delims);


/*  sep_line

    Given an input string (line) and a string of characters for separation
    (separatos), it separates the line and puts each string in a vector
    of strings (elements). It also computes the number of elements (n).

    Important: The line is useless after this function, so make sure to
    first make a copy if needed.

    Variables:
        -line = Input string to be separated.
        -separatos = Characters to separate the string.
        -elements = Output vector of strings.
        -n = number of elements obtained.
 
 */
void sep_line(char *line, char *separators, char ***elements, uint32_t *n);


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
node_t load_node(char *line, char *separator);


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
 
int compare_id(const void *node1, const void *node2);




/*  add_succ

    Adds a successor to a node, changing the size of the
    vector if necessary
 
    Variables:
        -node = node to add the successor.
        -vPos = position in the node vector.
 */
void add_succ(node_t *node, uint32_t id);


/*  add_way

    Computes for a way the nodes appearing in it, their position in the node
    vector, and depending of oneway or twoway, adds the edges to the nodes.
    
    Variables:
        -nodes = vector of nodes.
        -nnodes = number of nodes.
        -line = string of the way line.
        -separator = string to delimite the columns of the line.
 */
void add_way(node_t *nodes, uint32_t nnodes, char *line, char *separator);
