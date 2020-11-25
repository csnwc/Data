#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#define KEY_ESC 27

#define WARNBOXH 3
#define WARNBOXW 1024

#define NOPAIR -2

struct NCURS_Simplewin {
   bool finished;
   unsigned long attr[256]; /* Atrributes for each character */
   short pair[256]; /* Which Colour pair for each character */
   unsigned short pairsdefd;
};
typedef struct NCURS_Simplewin NCURS_Simplewin;

void Neill_NCURS_Delay(double ms);
void Neill_NCURS_Warning(char *s);
void Neill_NCURS_Events(NCURS_Simplewin *sw);
void Neill_NCURS_CharStyle(NCURS_Simplewin *sw, char *c, short fg, short bg, int attrs);
void Neill_NCURS_Done(void);
void Neill_NCURS_Init(NCURS_Simplewin *sw);
void Neill_NCURS_PrintArray(char *a, int width, int height, NCURS_Simplewin *sw);
void Neill_NCURS_PrintSubArray(char *a, int width, int height, int stride, NCURS_Simplewin *sw);
void Neill_NCURS_PrintLine(char *a, int width, int height,  NCURS_Simplewin *sw);
