/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);

  
  u_char a; 
  for (a=0; a<15; a++) 
  {
	  drawPixel(a,a,COLOR_BLACK); 
	  drawPixel(0,a,COLOR_RED); 
	  drawPixel(a,15,COLOR_GREEN);
  }
   
  
}
