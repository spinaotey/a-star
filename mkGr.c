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
