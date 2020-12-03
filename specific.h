
/* 128-bit number */
#define INITBYTES 16
#define SCALEFACTOR 2

#include <../../../ADTs/General/bool.h>

typedef unsigned char byte;

struct boolarr {
   /* Underlying Array */
   byte* a;
   /* Physical number of elements in underlying array */
   unsigned int nbytes;
   /* Number of bits stored */
   unsigned int nbits;
};
typedef struct boolarr boolarr;
