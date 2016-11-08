#include "mkGr.h"
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
void sep_line(char *line, char *separators, char ***elements, uint32_t *n){
    
    char *token;

    *n = 0;
    *elements = (char **) malloc(sizeof(char *)); assert(*elements);

    token = strtok(line,separators);

    while(token != NULL){
        (*n)++;
        *elements = (char **) realloc(*elements,sizeof(char *)*(*n));assert(*elements);
        (*elements)[(*n)-1]  = (char *) malloc(sizeof(char)*strlen(token)+1);assert((*elements)[(*n)-1]);
        strcpy((*elements)[(*n)-1],token);
        token = strtok(NULL,separators);
    }

}
