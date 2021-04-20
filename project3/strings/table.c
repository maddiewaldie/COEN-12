/* COEN 12 Lab #2 - File: unsorted.c
 * Author: Madeleine Waldie
 * Due Date: 5/2/21
 */

# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "set.h"

// Flag constant values
# define E 0 // empty
# define F 1 // filled
# define D 2 // deleted

typedef struct set {
	int count; // num elements
	int length; // length of array
	char **elts; // array
	char *flags; // array of flags (E - empty, F - filled, D - deleted)
} SET;

/*
strhash: assigns a unique hash value to each elt by running through each character in the elt, multiplying its value by 31, and adding those values together
runtime: O(1)
*/ 
unsigned strhash(char *s) {

	unsigned hash = 0;

	while(*s != '\0')
		hash = 31 * hash + *s ++;

	return hash;
}

/*
search: uses linear probing to find if an element is in the array, has been deleted from the array, or is not in the array
runtime: worst: O(n); expected: O(1)
*/ 
int search(SET *sp, char *elt, bool *found) {
	assert((sp != NULL) && (elt != NULL)); // make sure both sp & elt exist

	int h = strhash(elt) % sp->count;
	int i, loc;
	int deletedloc = -1;

	for(i = 0; i < sp->count; i++) {
		loc = (h + i) % sp->count;
		if(sp->flags[loc] == F) {
			if(strcmp(sp->elts[loc], elt) == 0) {
				*found = true;
				return loc;
			}
		}
		else if(sp->flags[loc] == D) {
			if(deletedloc == -1) {
				deletedloc = loc;
			}
		}
		else {
			break;
		}
	}

	*found = false;
	if(deletedloc == -1) {
		return loc;
	}
	return deletedloc;
	// return (deletedloc == -1) ? loc : deletedloc; // if deletedloc equals -1, return loc, otherwise return deletedloc
}

/*
createSet: return a pointer to a new set with a maximum capacity of maxElts
runtime: O(n)
*/
SET *createSet(int maxElts) {
	SET*sp = malloc(sizeof(SET)); // declare and allocate memory for set
	assert(sp != NULL); // check to make sure memory was allocated

	sp->count = 0; // set count to number of elements currently in set (0 right now)
	sp->length = maxElts; // set length to length of array
	sp->elts = malloc(sizeof(char*)*maxElts); //allocate memory for array

	int i;
	for(i = 0; i < maxElts; i++) {
		sp->flags[i] = E; // assign each empty elt an empty flag
	}

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
		if(sp->flags[i] == F) {
			free(sp->elts[i]); // deallocate memory of filled slots in array
		}
	}
	free(sp->flags); //deallocate memory of the array of flags
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
runtime: worst: O(n); expected: O(1)
*/
void addElement(SET *sp, char *elt) {
	assert(sp != NULL && elt != NULL);

	bool found;
	int index = search(sp, elt, &found);
	if (!found) {
		sp->elts[index] = strdup(elt);
		sp->flags[index] = F;
		sp->count++;
	}
}

/*
removeElement: remove elt from the set pointed to by sp
runtime: worst: O(n); expected: O(1)
*/
void removeElement(SET *sp, char *elt) {
	assert(sp != NULL && elt != NULL);

	bool found;
	int index = search(sp, elt, &found);
	if (found) {
		free(sp->elts[index]);
		sp->flags[index] = D;
		sp->count--;
	}
}

/*
findElement: if elt is present in the set pointed to by sp then return the matching element, otherwise return NULL
runtime: worst: O(n); expected: O(1)
*/
char *findElement(SET *sp, char *elt) {
	assert(sp != NULL && elt != NULL);

	bool found;
	int index = search(sp, elt, &found);

	if (found == true) {
		return sp -> elts[index];
	} else {
		return NULL;
	}
	// return (found) ? sp->elts[index] : NULL; // if elt is found, return elt, otherwise return NULL
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
	int indx = 0;
	for(i = 0; i < sp->count; i ++) {
		if(sp->flags[i] == F) {
			copy[indx] = strdup(sp->elts[i]); // copy each elt into a copy array
			indx++;
		}
	}

	return copy;
}
