#include "shape.h"



int doSomethingCrazy(const AbSpaceInvadey *si, const Vec2 *centerPos, const Vec2 *pixel) {
    int centerX = centerPos->axes[0];
    int centerY = centerPos->axes[1];
    int pixelX = pixel->axes[0];
    int pixelY = pixel->axes[1];

    return 0;

}

int abSpaceInvadeyCheck(const AbSpaceInvadey *si, const Vec2 *centerPos, const Vec2 *pixel) {
  int centerX = centerPos->axes[0];
  int centerY = centerPos->axes[1];
  int pixelX = pixel->axes[0];
  int pixelY = pixel->axes[1];

  
  // Draw point on head
  if (pixelY == centerY-3 && pixelX == centerX)
    return 1;

  // Draw row above eyes
  if (pixelY == centerY-2 && pixelX >= centerX-2 && pixelX <= centerX+2)
    return 1;

  // Draw row with eyes
  if (pixelY == centerY-1 && (pixelX == centerX-2 || pixelX == centerX || pixelX == centerX+2))
    return 1;

  // Draw middle row
  if (pixelY == centerY && pixelX >= centerX-3 && pixelX <= centerX+3)
    return 1;

  // Draw legs first part
  if (pixelY == centerY+1 && (pixelX == centerX-1 || pixelX == centerX+1))
    return 1;

  // Draw legs second part
  if (pixelY == centerY+2 && (pixelX == centerX-2 || pixelX == centerX || pixelX == centerX+2))
    return 1;
  
  // Draw legs third part
  // if (pixelY == centerY+3 && (pixelX == centerX-3 || pixelX == centerX-1
  //|| pixelX == centerX+1 || pixelX == centerX+3))
  //return 1;
  
  return 0;
}


/*
 * Get the boundary of the shape. The size of the shape is a fixed 7x7 so taking
 * the center position and adding/subtracting 3x3 will give us a box.
 */
void abSpaceInvadeyGetBounds(const AbSpaceInvadey *si, const Vec2 *centerPos, Region *bounds) {
  Vec2 half = {3,3};
  vec2Sub(&bounds->topLeft, centerPos, &half);
  vec2Add(&bounds->botRight, centerPos, &half);
}
