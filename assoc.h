/*
   A Hash Table, storing void pointers to a key/data pair.
   The user is responsible for keeping the data persistent
   (i.e. pointing at objects that haven't been freed)
   No copying of key/data is done.
*/

#include "../../ADTs/General/general.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

/*
   Initialise the Associative array
   keysize : number of bytes (or 0 => string)
   This is important when comparing keys since
   we'll need to use either memcmp() or strcmp()
*/
assoc* assoc_init(int keysize);

/*
   Insert key/data pair
   - may cause resize, therefore 'a' might
   be changed due to a realloc() etc.
*/
void assoc_insert(assoc** a, void* key, void* data);

/*
   Returns the number of key/data pairs 
   currently stored in the table
*/
unsigned int assoc_count(assoc* a);

/*
   Returns a pointer to the data, given a key
   NULL => not found
*/
void* assoc_lookup(assoc* a, void* key);

void assoc_todot(assoc* a);

/* Free up all allocated space from 'a' */
void assoc_free(assoc* a);
