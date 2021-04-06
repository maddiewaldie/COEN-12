/* COEN 12 Lab #2 - File: unsorted.c
 * Author: Madeleine Waldie
 * Due Date: 4/18/21
 */

# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "set.h"

# define MAX_SIZE 18000

typedef struct set {
    int count; // num elements
    int length; // length of array
    char **elts; // array
} SET;

/*
    search: private function that sequentially searches through array for elt; if element is not found, return -1.
    runtime: O(n)
*/ 
int search(SET *sp, char *elt) {
    assert((sp != NULL) && (elt != NULL)); // make sure both sp & elt exist

    int i;
    for(i = 0; i < sp -> count; i++) {
        if (strcmp(sp->elts[i],elt)==0) {
            return i; // return index of elt
        }
    }

    return -1; // if element is not found return -1
}

/*
    createSet: return a pointer to a new set with a maximum capacity of maxElts
    runtime: O(1)
*/
SET *createSet(int maxElts) { 
    SET*sp = malloc(sizeof(SET)); // declare and allocate memory for set
    assert(sp != NULL); // check to make sure memory was allocated

    sp->count = 0; // set count to number of elements currently in set (0 right now)
    sp->length = maxElts; // set length to length of array
    sp->elts = malloc(sizeof(char*)*maxElts); //allocate memory for array
    assert(sp->elts != NULL); // check to make sure memory was allocated

    return sp;
}

/*
    destroySet: deallocate memory associated with the set pointed to by sp
    runtime: O(n)
*/
void destroySet(SET *sp) { 
    int i;
    for(i = 0; i < sp->count; i++) {
        free(sp->elts[i]); // deallocate memory of elements in array
    }
    
    free(sp->elts); // deallocate memory of the array
    free(sp); //  deallocate memory of the set
}

/*
    numElements: return the number of elements in the set pointed to by sp
    runtime: O(1)
*/
int numElements(SET *sp) {
    assert(sp != NULL); // make sure sp exists
    return sp->count;
}

/*
    addElement: add elt to the set pointed to by sp
    runtime: O(n)
*/
void addElement(SET *sp, char *elt) { 
    assert((sp != NULL) && (elt != NULL) && (sp->count < MAX_SIZE)); // make sure both sp & elt exist, and that count is less than max num of elements
    if(search(sp, elt) == -1) { // only add elt if there isn't a duplicate of elt in sp
        sp->elts[sp -> count] = strdup(elt);
        sp->count++;
    }
}

/*
    removeElement: remove elt from the set pointed to by sp
    runtime: O(n)
*/
void removeElement(SET *sp, char *elt) {
    assert((sp != NULL) && (elt != NULL)); // make sure both sp & elt exist

    int i = search(sp, elt); // get index of elt

    if(i != -1) { // only remove elt if elt exists in the array
        free(sp->elts[i]); // deallocate memory at index of elt
        sp->elts[i] = sp->elts[sp->count - 1]; // put last element into the slot elt was previously in
        sp->elts[sp->count - 1] = NULL; // set last element to null
        sp->count--; // decrease count by 1
    }
}

/*
    findElement: if elt is present in the set pointed to by sp then return the matching element, otherwise return NULL
    runtime: O(n)
*/
char *findElement(SET *sp, char *elt) {
    assert((sp != NULL) && (elt != NULL)); // make sure both sp & elt exist
    
    return (search(sp, elt) == -1) ? NULL : elt; // if elt isn't present in set, return NULL, otherwise return elt
}

/*
    getElements: allocate and return an array of elements in the set pointed to by sp
    runtime: O(n)
*/
char **getElements(SET *sp) {
    assert(sp != NULL); // make sure sp exists

    char **copy = malloc(sizeof(char*)*sp->count);
    assert(copy != NULL); // check to make sure memory was allocated
    
    int i;
    for(i = 0; i < sp->count; i ++) {
        copy[i] = strdup(sp->elts[i]); // copy each elt into a copy array
    }

    return copy;
}
