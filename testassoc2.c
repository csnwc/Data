#include "specific.h"
#include "assoc.h"

#define WORDS 500000
#define WORDLEN 50

char* strduprev(char* str);

#define FNAMELEN 100

#define STRINGKEY 0

int add_dictionary(assoc** b, char* fname, char words[WORDS][WORDLEN]);

int main(void)
{

   static char strs[WORDS][WORDLEN]={{0}};
   int i;
   assoc* a;
   FILE* fp;
   char fname1[FNAMELEN];
   char fname2[FNAMELEN];

   strcpy(fname1, "../../Data/Words/eng_370k_shuffle.txt");
   a = assoc_init(STRINGKEY);
   i = add_dictionary(&a, fname1, strs);
   /* Read words in 'Pride & Prejudice' */
   strcpy(fname2, "../../Data/Words/p-and-p-words.txt");
   fp = nfopen(fname2, "rt");
   while(fscanf(fp, "%s", strs[i])==1){
      if(assoc_lookup(a, strs[i])==NULL){
         assoc_insert(&a, strs[i], fname2);
         i++;
      }
   } 
   fclose(fp);

   printf("tulip added from %s\n", (char*)assoc_lookup(a, "tulip"));
   printf("goulding added from %s\n", (char*)assoc_lookup(a, "goulding"));
   assoc_free(a);
   return 0;
}

int add_dictionary(assoc** b, char* fname, char words[WORDS][WORDLEN])
{
   FILE* fp;
   int i = 0;
   assoc* a = *b;

   /* Dictionary */
   fp = nfopen(fname, "rt");
   while(fscanf(fp, "%s", words[i])==1){
      assoc_insert(&a, words[i], fname);
      i++;
   }
   fclose(fp);
   *b = a;
   return i;
}


/* Make a copy, reversed */
char* strduprev(char* str)
{
   int i, j;
   char* t;
   j = strlen(str);
   t = ncalloc(j+1, 1); /* Add null char */
   strcpy(t, str);
   for(i=0, j--; i<j; i++,j--){
      /* Swap using bit-twiddling */
      t[i] ^= t[j];
      t[j] ^= t[i];
      t[i] ^= t[j];
   }
   return t;
}
