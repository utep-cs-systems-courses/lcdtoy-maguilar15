#include "shape.h"


void drawBackground(const DemoShape *si, const Vec2 *centerPos, Region *bounds)
{
    Vec2 half = {2,2};
    vec2Sub(&bounds->topLeft, centerPos, &half);
    vec2Add(&bounds->botRight, centerPos, &half);
}


int demoShape1(const DemoShape *si, const Vec2 *centerPos, const Vec2 *pixel)
{
    int centerX = centerPos->axes[0];
    int centerY = centerPos->axes[1];
    int pixelX = pixel->axes[0];
    int pixelY = pixel->axes[1];

    return 0;

}
