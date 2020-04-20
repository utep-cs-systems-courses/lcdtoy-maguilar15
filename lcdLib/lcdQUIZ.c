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

 
  u_char j;
  u_char i; 
  for (i=0; i<20; i++)
  {
  for (j=0; j<15; j++)
  {
  	drawPixel(i,i,COLOR_BLACK);
        drawPixel(j,j,COLOR_BLACK); 
	drawPixel(i,j,COLOR_BLACK); 	
	
  }
  } 
  
}
