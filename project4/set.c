/* COEN 12 Lab #4 - File: set.c
 * Author: Madeleine Waldie
 * Due Date: 5/16/21
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include "set.h"
# include <stdbool.h>
# include "list.h"

struct set{
    int count;
    int length;
    LIST  **data;
    int (*compare)();
    unsigned (*hash)();
};

SET *createSet(int maxElts,int (*compare)(),unsigned (*hash)());
void destroySet(SET *sp);
int numElements(SET *sp);
void addElement(SET *sp,void  *elt);
void removeElement(SET *sp, void  *elt);
void  *findElement(SET *sp, void *elt);
void  *getElements(SET *sp);