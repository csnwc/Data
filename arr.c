#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arr.h"

/* Default physical size of array -
   no need to share this */
#define ARRDEFSZ 32

/* Set up array with no elements, but a physical container having
   a default ARRDEFSZ size */
arr* arr_init(void)
{
   arr* l;
   l = (arr*) calloc(1,sizeof(arr));
   if(l == NULL){
      ON_ERROR("Creation of Array Failed\n");
   }
   l->data = (arrtype*) calloc(ARRDEFSZ, sizeof(arrtype));
   if(l->data == NULL){
      ON_ERROR("Creation of Array Failed\n");
   }
   l->pz = ARRDEFSZ;
   return l;
}

/* No array *written* out-of-bounds, resize */
void arr_set(arr *l, int n, arrtype i)
{
   if (n < 0){
      ON_ERROR("Array accessed with negative index ...\n");
   }
   /* Index greater than current physical size, expand */
   if(n >= l->pz){
      /* Like malloc, but copies original data, then frees original */
      l->data = (arrtype*) realloc(l->data, sizeof(arrtype)*(n+1));
      if(l->data == NULL){
         ON_ERROR("Resize of Array Failed\n");
      }
      l->pz = n+1;
   }
   l->data[n] = i;
}

/* Error when array *read* out-of-bounds, unlike set */
arrtype arr_get(arr *l, int n)
{
   if((n >= l->pz) || (n < 0)){
      ON_ERROR("Array read out of bounds\n");
   }
   return l->data[n];
}

/* Pass in the pointer to the arr pointer,
   so it can be set to NULL after */
void arr_free(arr **p)
{
   arr* a = *p;
   free(a->data);
   free(a);
   /* Helps to assert that the job has been done.*/
   *p = NULL;
}
