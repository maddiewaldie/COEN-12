/* COEN 12 Lab #2 - File: sorted.c
 * Author: Madeleine Waldie
 * Due Date: 4/18/21
 */

# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "set.h"

# define MAX_SIZE 18000

typedef struct set {
    int count; // num elements
    int length; // length of array
    char **elts; // array
} SET;

/*
    search: private function that uses binary search to go through array for elt
    runtime: O(log(n))
*/
int search(SET *sp, char *elt, bool *found) {
    assert((sp != NULL) && (elt != NULL)); // make sure both sp & elt exist
    
    int low = 0, high = sp->count-1, mid;

    while(low <=high) {
        mid = (high+low)/2;

        if(strcmp(sp->elts[mid], elt) > 0) {
            high = mid - 1;
        }
        else if(strcmp(sp->elts[mid], elt) < 0) {
            low = mid+1;
        }
        else {
            *found = true;
            return mid;
        }
    }

    *found = false;
    return low;
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
    runtime: O(nlog(n))
*/
void addElement(SET *sp, char *elt) { 
    assert((sp != NULL) && (elt != NULL) && (sp->count < MAX_SIZE)); // make sure both sp & elt exist, and that count is less than max num of elements
    
    bool found;
    int i = search(sp, elt, &found), j;

    if(found == false) { // only add elt if there isn't a duplicate of elt in sp
        
        for(j = sp -> count; j > i; j--) {
            sp->elts[j] = sp->elts[j-1]; // shift everything to the right, freeing up a space for elt
        }

        sp->elts[i] = strdup(elt);
        sp->count++;
    }
}

/*
    removeElement: remove elt from the set pointed to by sp
    runtime: O(nlog(n))
*/
void removeElement(SET *sp, char *elt) {
    assert((sp != NULL) && (elt != NULL)); // make sure both sp & elt exist

    bool found;
    int i = search(sp, elt, &found), j; // get index of elt

    if(found) { // only remove elt if elt exists in the array
        free(sp->elts[i]); // deallocate memory at index of elt
        for(j = i; j < sp->count; j++) {
            sp->elts[j] = sp->elts[j+1]; // shift everything to the left, filling up the empty space
        }

        sp->count--;
    }
}

/*
    findElement: if elt is present in the set pointed to by sp then return the matching element, otherwise return NULL
    runtime: O(log(n))
*/
char *findElement(SET *sp, char *elt) {
    assert((sp != NULL) && (elt != NULL)); // make sure both sp & elt exist
    
    bool found;
    search(sp, elt, &found);
    return found ? elt : NULL; // if elt isn't present in set, return NULL, otherwise return elt
}

/*
    getElements: allocate and return an array of elements in the set pointed to by sp
    runtime: O(n)
*/
char **getElements(SET *sp) {
    assert(sp != NULL); // make sure both sp exists

    char **copy = malloc(sizeof(char*)*sp->count);
    assert(copy != NULL); // check to make sure memory was allocated
    
    int i;
    for(i = 0; i < sp->count; i ++) {
        copy[i] = strdup(sp->elts[i]); // copy each elt into a copy array
    }

    return copy;
}
