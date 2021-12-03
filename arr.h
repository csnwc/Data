#ifndef __ARR_H__
#define __ARR_H__

/*
A resizeable indexed array ADT. Here:
1. Setting an element out of bounds reallocs the array to make it valid
2. Getting an element out-of-bounds is an error
3. The type is defined by using
   -D ARRINTS or -D ARRSTRINGS from the compile line.
*/

#ifdef ARRINTS
   typedef int arrtype;
#endif
#ifdef ARRSTRINGS
   typedef struct arrtype {char str[50];} arrtype;
#endif

/* What to do on error */
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

/* Main structure for array - pz is current (max) size
   data points to the alloc()'d space to use as the array */
struct arr {
   arrtype *data;
   int pz;
};
typedef struct arr arr;

/* Creates the empty array */
arr* arr_init(void);
/* Similar to l[n] = i, safely resizing if required */
void arr_set(arr *l, int n, arrtype i);
/* Similar to = l[n] */
arrtype arr_get(arr *l, int n);
/* Clears all space used, and sets pointer to NULL */
void arr_free(arr **l);

#endif
