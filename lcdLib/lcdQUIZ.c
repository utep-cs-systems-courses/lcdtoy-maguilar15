/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);

  u_char a,c,colLeft,colRight; 
 
  for (a = 0; a <= 15; a += 3) 
  {
	  colLeft = 15 - a; 
	  colRight = 15; 

  	for (c = colLeft; c <= colRight; c += 3) 
	{
		drawPixel(20+c,a+30,COLOR_RED);
		//drawPixel(c+20,a,COLOR_RED); // WOW 
		drawPixel(20-c,a+30,COLOR_BLACK); // N/A
		drawPixel(20+c,30-a,COLOR_GREEN);
	        drawPixel(20-c,30-a,COLOR_YELLOW); 	
	}
  }

  /*
  for (a=0; a<=10; a++)
	  drawPixel(10,a,COLOR_RED);
  for (a=0; a<=10; a++)
	  drawPixel(a,10,COLOR_BLACK); 
  */

  /*
  u_char a; 
  for (a=0; a<15; a++) 
  {
	  drawPixel(a,a,COLOR_BLACK); 
	  drawPixel(0,a,COLOR_RED); 
	  drawPixel(a,15,COLOR_GREEN);
  }
 */


  /*
 u_char i; 
 u_char j; 
    for (i=0;i<10;i++)
  {
          for(j=0; j<40; j++)
          {
          drawPixel(i,i,COLOR_BLACK);
          drawPixel(0,j,COLOR_RED);
	  drawPixel(j,20,COLOR_GREEN);
          }
  }
  */

  
}
