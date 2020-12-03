#include "specific.h"
#include "../boolarr.h"

bool _xor(bool b1, bool b2);
bool _or(bool b1, bool b2);
bool _and(bool b1, bool b2);
void _whichbitbyte(const int n, unsigned int* whichbit, unsigned int* whichbyte);

boolarr* boolarr_init(void)
{
   boolarr* ba;
   ba = ncalloc(1, sizeof(boolarr));
   ba->a = ncalloc(1, INITBYTES);
   ba->nbytes = INITBYTES;
   return ba;
}

unsigned int boolarr_size(const boolarr* ba)
{
   if(ba==NULL){
      return 0;
   }
   return ba->nbits;
}

boolarr* boolarr_clone(const boolarr* ba)
{
   boolarr* cp;
   if(ba == NULL){
      return NULL;
   }
   cp = ncalloc(1, sizeof(boolarr));
   memcpy(cp, ba, sizeof(boolarr));
   cp->a = ncalloc(1, ba->nbytes);
   memcpy(cp->a, ba->a, ba->nbytes);
   return cp;
}

boolarr* boolarr_initstr(const char* str)
{
   int i;
   int nbits;
   boolarr* bb;
   if(str==NULL){
      return false;
   }
   bb = boolarr_init();
   nbits = strlen(str);
   if(bb != NULL){
      for(i=nbits-1; i>=0; i--){
        switch(str[nbits-i-1]){
           case '1':
              if(!boolarr_set(bb, i, true)){
                 return false;
              }
              break;
           case '0':
              if(!boolarr_set(bb, i, false)){
                 return false;
              }
              break;
           default:
              return false;
         }
      }
   }   
   return bb;
}

bool boolarr_tostring(const boolarr* ba, char* str)
{
   int i;
   bool b;
   if((ba==NULL) || (str==NULL) || (boolarr_size(ba)==0)){
      return false;
   }
   str[0] = '\0';

   for(i=ba->nbits-1; i>=0; i--){
     if(boolarr_get(ba, i, &b)){
        if(b){
           str[ba->nbits-i-1] = '1';
        }
        else{
           str[ba->nbits-i-1] = '0';
        }
      }
   }
   str[ba->nbits] = '\0';
   return true;
}

bool boolarr_print(const boolarr* ba)
{
   int i;
   char* str;
   if((ba==NULL) || (boolarr_size(ba)==0)){
      return false;
   }
   str = calloc(ba->nbits+1, 1);
   printf("Structure %p (underlying array %p)\n", (void*)ba, (void*)ba->a);
   printf("Total bits stored = %d\n", ba->nbits);
   printf("Total bytes = %d\n", ba->nbytes);
   printf("Binary ");
   boolarr_tostring(ba, str);
   for(i=0; str[i]; i++){
      putchar(str[i]);
   }
   printf("\n");
   free(str);
   return true;
}

bool boolarr_get(const boolarr* ba, const unsigned int n, bool* b)
{

   unsigned int whichbyte;
   unsigned int whichbit; 
   byte res;
   byte mask;
   if((ba==NULL) || (n >= ba->nbits)){
      return false;
   }
   /* Array is laid out like as :
   bit  : 76543210 15....8 23...16
   byte :     0       1       2
   */
   _whichbitbyte(n, &whichbit, &whichbyte);
   mask = 1<<whichbit;
   res = ba->a[whichbyte] & mask;
   if(res){
      *b = true;
   }
   else{
      *b = false;
   }
   return true;
   
}

bool boolarr_set(boolarr* ba, const unsigned int n, const bool b)
{
   unsigned int whichbyte;
   unsigned int whichbit; 
   byte res;
   byte mask;
   if(ba==NULL){
      return false;
   }
   _whichbitbyte(n, &whichbit, &whichbyte);
   /* Resize */
   if(n >= ba->nbits){
      ba->nbits = n+1;
   }
   while(whichbyte >= ba->nbytes){
      /* Setting a bit out-of-bounds for our current array */
      ba->a = nrecalloc(ba->a, ba->nbytes, ba->nbytes*SCALEFACTOR);
      ba->nbytes *= SCALEFACTOR;
   }
   mask = 1<<whichbit;
   if(b){
      /* Set 1 */
      res = ba->a[whichbyte] | mask;
   }
   else{
      /* Set 0 */
      res = ba->a[whichbyte] & ~mask;
   }
   ba->a[whichbyte] = res;
   return true;
}
boolarr*  boolarr_bitwise(const boolarr* ba, const boolarr* bb, const logicalop lo)
{

   unsigned int i;
   boolarr* bc;
   bool b1, b2, br;
   /* Array of function pointers */
   bool(*f[])(bool, bool) = {_or, _and, _xor};
   
   if((ba==NULL) || (boolarr_size(ba)==0) ||
      (boolarr_size(ba) != boolarr_size(bb))){
      return NULL;
   }
   bc = boolarr_clone(ba);
   if((bc==NULL)){
      return NULL;
   }
   for(i=0; i<ba->nbits; i++){
      if(!boolarr_get(ba, i, &b1) || !boolarr_get(bb, i, &b2)){
        /* Hmm, something went wrong with the get ?*/
        boolarr_free(bc);
        return NULL;
      }
      /* Which particular logic op to be applied ? */
      br = f[lo](b1, b2);
      if(!boolarr_set(bc, i, br)){
        /* Hmm, something went wrong with the set ?*/
        boolarr_free(bc);
        return NULL;
      }
   }
   return bc;
}

bool boolarr_negate(boolarr* ba)
{
   unsigned int i;
   bool b;
   if((ba==NULL) || (boolarr_size(ba)==0)){
      return false;
   }
   for(i=0; i<ba->nbits; i++){
      if(boolarr_get(ba, i, &b)){
         if(b){
            if(!boolarr_set(ba, i, false)){
               return false;
            }
            boolarr_get(ba, i, &b);
         }
         else{
            if(!boolarr_set(ba, i,  true)){
               return false;
            }
            boolarr_get(ba, i, &b);
         }
      }
   }
   return true;
}

unsigned int boolarr_count1s(const boolarr* ba)
{

   bool b;
   unsigned int i;
   unsigned int cnt = 0;
   if((ba==NULL) || (boolarr_size(ba)==0)){
      return 0;
   }
   for(i=0; i<ba->nbits; i++){
      if(boolarr_get(ba, i, &b)){
         if(b){
            cnt++;
         }
      }
   }
   return cnt;
}

bool boolarr_issame(const boolarr* b1, const boolarr* b2)
{
   unsigned int i;
   bool bl1, bl2;
   if(b1 == b2){
      return true;
   }
   if(boolarr_size(b1) != boolarr_size(b2)){
      return false;
   }
   if((b1==NULL) || (b2==NULL)){
      return false;
   }
   for(i=0; i<b1->nbits; i++){
      if((!boolarr_get(b1, i, &bl1)) || 
         (!boolarr_get(b2, i, &bl2))){
         return false;
      }
      if(bl1 != bl2){
         return false;
      }
   }
   return true;
}

bool boolarr_free(boolarr* ba)
{
   if(ba==NULL){
      return false;
   }
   free(ba->a);
   free(ba);
   return true;
}

/**********************/
/* Auxiliary Function */
/**********************/

bool _xor(bool b1, bool b2)
{
   return (b1 ^ b2);
}

bool _and(bool b1, bool b2)
{
   return (b1 & b2);
}

bool _or(bool b1, bool b2)
{
   return (b1 | b2);
}

/* Num bits per byte */
#define BPB 8
void _whichbitbyte(const int n, unsigned int* whichbit, unsigned int* whichbyte)
{
   *whichbit  = n%BPB;
   *whichbyte = n/BPB;
}
