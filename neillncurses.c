#include "neillncurses.h"

void Neill_NCURS_Delay(double ms){

    const clock_t start = clock();
    clock_t current;
    do{
        current = clock();
    }while((double)(current-start)/CLOCKS_PER_SEC < (ms/1000.0));

}

/* Display a warning (.e.g array too large to fit on screen */
void Neill_NCURS_Warning(char *s)
{

   int i, lx, ln;
   chtype lne[WARNBOXH][WARNBOXW];

   /* Default */
   attrset(A_STANDOUT|COLOR_PAIR(0));
   ln = strlen(s);
   /* Halfway across screen */
   lx = (COLS-ln)/2;
   /* Store contents of screen */
   for(i=0; i<WARNBOXH; i++){
      mvinchnstr(i, lx, lne[i], ln);
   }
   /* Draw lines */
   mvhline( 0, lx, 0, ln);
   mvhline( 2, lx, 0, ln);
   mvprintw(1, lx, s);
   refresh();
   /* 4 Seconds */
   Neill_NCURS_Delay(4000.0);
   /* Restore contents of screen */
   for(i=0; i<WARNBOXH; i++){
      mvaddchnstr(i, lx, lne[i], ln);
   }
   refresh();


}

/* Get mouse / key events & set sw.finished if appropriate */
void Neill_NCURS_Events(NCURS_Simplewin *sw)
{
   int ch;
   /* Keep processing events until none queued */
   while((ch = getch()) != ERR){
      switch(ch){
         /* Any mouse press */
         case KEY_MOUSE:
         /* ESC key pressed - why not defined in ncurses.h ? */
         case KEY_ESC:
         case 'q':
         case 'Q':
              sw->finished = TRUE;
              break;
      }
   }
}


/*
   Make sure that all characters in string c are displayed in foreground colour fg,
   background colour bg, with additional style attrs (which may be or'd together)
   Colours can include :
   COLOR_BLACK, COLOR_RED, COLOR_GREEN,COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE
   Attributes can include (but many may not be implemented by your terminal):
   A_ALTCHARSET, A_BOLD, A_DIM, A_INVIS, A_PROTECT, A_REVERSE, A_STANDOUT, A_UNDERLINE
*/
void Neill_NCURS_CharStyle(NCURS_Simplewin *sw, char *c, short fg, short bg, int attrs)
{

   int i;
   /* See if fg/bg are already a defined pair */
   short ef, eb;
   short pr = NOPAIR;
   for(i=1; i<=sw->pairsdefd; i++){
      pair_content(i, &ef, &eb);
      /* Found existing pair */
      if(ef==fg && eb==bg){
         pr = i;
      }
   }
   /* No pair found */
   if(pr == NOPAIR){
      sw->pairsdefd++;
      pr = sw->pairsdefd;
      init_pair(pr, fg, bg);
   }

   for(i=0; c[i] > '\0'; i++){
      /* A_BOLD | A_REVERSE | A_DIM | A_UNDERLINE etc. */
      sw->attr[(int)c[i]] = attrs;
      sw->pair[(int)c[i]] = pr;
   }

}

void Neill_NCURS_Done(void)
{
   /* Should deal with return value ? */
   endwin();
}

void Neill_NCURS_Init(NCURS_Simplewin *sw)
{

   int i;
   /* Global var `stdscr` is assigned by the call to initscr(), nasty ;-)
      Even manual says no point in keeping return value, hence cast */
   (void)initscr();
   clear();
   start_color();	
   if(has_colors() == FALSE){
        endwin();
	fprintf(stderr, "Your terminal does not support colour\n");
	exit(1);
   }
   sw->pairsdefd = 0;
   /* All Characters default colour */
   for(i=0 ;i<256; i++){
      sw->pair[i] = 0;
      sw->attr[i] = A_NORMAL;
   }
   cbreak();
   nodelay(stdscr,TRUE);
   noecho();
   curs_set(FALSE);
   sw->finished = FALSE;
   /* Capture all mouse events */
   mousemask(ALL_MOUSE_EVENTS, NULL);
   /* Flush key presses on CTRL-C / Break etc. */
   intrflush(NULL, TRUE);

}

/* a : Pointer to first character in 1D or 2D array
   width, height of array to be displayed
*/
void Neill_NCURS_PrintArray(char *a, int width, int height, NCURS_Simplewin *sw)
{
   Neill_NCURS_PrintSubArray(a, width, height, width, sw);
}

/* Stride : When displaying a subpart of an array the displayed width might be
   different from the true array size.*/
void Neill_NCURS_PrintSubArray(char *a, int width, int height, int stride, NCURS_Simplewin *sw)
{

   int i, j;
   static char *preva = NULL;
   /* No colour pair yet */
   short cp = NOPAIR;
   /* No Attr yet */
   unsigned long ap = A_NORMAL;
   int termx, termy;
   /* Macro not function, no pointers required. */
   getmaxyx(stdscr, termy, termx);
   if(width > termx || height > termy){
      /* Don't repeat same warning for same array.Nasty. */
      if(preva != a){
         char str[256];
         sprintf(str, "Terminal (%d,%d) too small for array (%d %d)", termx, termy, width, height); 
         Neill_NCURS_Warning(str);
         preva = a;
      }
   }
   clear();
   for(j=0; j<height; j++){
      move(j,0);
      for(i=0; i<width; i++){
         int c = a[j*stride+i];
         /* This colour/attribute different from last */
         if((sw->pair[c] != cp) || (sw->attr[c] != ap)){
            cp = sw->pair[c];
            ap = sw->attr[c];
            attrset(ap | COLOR_PAIR(cp));
         }
         addch(c);
      }
   }
   refresh();

}
