/** \file shapemotion.c
 *  \brief This is a simple shape motion demo.
 *  This demo creates two layers containing shapes.
 *  One layer contains a rectangle and the other a circle.
 *  While the CPU is running the green LED is on, and
 *  when the screen does not need to be redrawn the CPU
 *  is turned off along with the green LED.
 */

#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include "buzzer.h"

#define GREEN_LED BIT6


/** OBJECTS :  lcdtoy-maguilar15/project/shapeLib/shapedemo2.c  */
AbRect rect10 = {abRectGetBounds, abRectCheck, {10,10}}; /**< 10x10 rectangle */
AbRect object1 = {abRectGetBounds, abRectCheck, {10,10}};
AbRect object2 = {abRectGetBounds, abRectCheck, {10,10}};
AbRect object3 = {abRectGetBounds, abRectCheck, {10,10}};
AbRect object4 = {abRectGetBounds, abRectCheck, {10,10}};
AbRect object5 = {abRectGetBounds, abRectCheck, {10,10}};
AbRect object6 = {abRectGetBounds, abRectCheck, {10,10}};


AbRectOutline fieldOutline = {/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,
  {screenWidth/2 - 20, screenHeight/2 - 15}
};

Layer layer8 = {
        (AbShape *)&rect10,
        {(screenWidth/2)+10, (screenHeight/2)+5},
        {0,0}, {0,0},    /* last & next pos */
        COLOR_PINK,
        0
};


Layer layer7 = {
        (AbShape *)&object6,
        {screenWidth/2+20, screenHeight/2+3},
        {0,0}, {0,0},    /* last & next pos */
        COLOR_BLACK,
        &layer8,
};


Layer layer6 = {
        (AbShape *)&object5,
        {screenWidth/2+10, screenHeight/2+30},
        {0,0}, {0,0},    /* last & next pos */
        COLOR_BLACK,
        &layer7,
};



Layer layer5 = {
        (AbShape *)&object4,
        {screenWidth/2+0, screenHeight/2+13},
        {0,0}, {0,0},
        COLOR_BLACK,
        &layer6,
};

Layer layer4 = {
        (AbShape *)&object3,
        {screenWidth/2+20, screenHeight/2+3},
        {0,0}, {0,0},
        COLOR_BLACK,
        &layer5,
};

Layer layer3 = {
        (AbShape *)&circle8,
        {(screenWidth/2)+10, (screenHeight/2)+5},
        {0,0}, {0,0},
        COLOR_BLUE,
        &layer4,
};

Layer fieldLayer = {
        (AbShape *) &fieldOutline,
        {screenWidth/2, screenHeight/2},/**< center */
        {0,0}, {0,0},
        COLOR_BLACK,
        &layer3
};


Layer layer1 = {
        (AbShape *)&object1,
        {screenWidth/2, screenHeight/2}, /**< center */
        {0,0}, {0,0},
        COLOR_BLACK,
        &fieldLayer,
};

Layer layer0 = {
        (AbShape *)&object2,
        {(screenWidth/2)+10, (screenHeight/2)+5},
        {0,0}, {0,0},
        COLOR_BLACK,
        &layer1,
};




/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

/* Layers that will move / initial value of {0,0} will be overwritten */
MovLayer ml7 = { &layer7, {1,1}, 0};
MovLayer ml6 = { &layer6, {1,1}, &ml7};
MovLayer ml5 = { &layer5, {2,2}, &ml6};
MovLayer ml4 = { &layer4, {1,2}, &ml5};
MovLayer ml3 = { &layer3, {1,1}, &ml4}; 
MovLayer ml1 = { &layer1, {1,2}, &ml3};
MovLayer ml0 = { &layer0, {2,1}, &ml1};

void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);/**< disable interrupts (GIE on) */
  
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1],
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer;
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break;
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color);
      } // for col
    } // for row
  } // for moving layer being updated
}

/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
 */
void mlAdvance(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	newPos.axes[axis] += (2*velocity);
      }/**< if outside of fence */
    } /**< for axis */
    ml->layer->posNext = newPos;
  } /**< for ml */
}
u_int bgColor = COLOR_WHITE;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */


Region fieldFence;/**< fence around playing field  */


/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */

void main()
{
  
  P1DIR |= GREEN_LED;/**< Green led on when CPU on */
  P1OUT |= GREEN_LED;

  buzzer_init();
  configureClocks();
  lcd_init();
  shapeInit();
  p2sw_init(15);
  shapeInit();
  layerInit(&layer0);
  layerDraw(&layer0);
  layerGetBounds(&fieldLayer, &fieldFence);

  enableWDTInterrupts();      /**< enable periodic interrupt */

  or_sr(0x8);              /**< GIE (enable interrupts) */

  
  for(;;) {
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    movLayerDraw(&ml0, &layer0);
  }
}


/** Watchdog timer 10 second interrupt */
void wdt_c_handler()
{
  unsigned int switches = p2sw_read();

  static short count = 0;
  
  P1OUT |= GREEN_LED;      /**< Green LED on when cpu on */
  count ++;
  if (count == 10)
  {

    buzzer_set_period(0);
    
    mlAdvance(&ml0, &fieldFence);

    if (!(switches & BIT0)) {
      redrawScreen = 1;
      drawString8x12(10,5,"LOUD NOISES!",COLOR_BLACK, COLOR_WHITE);    /* FONT REQUIREMENT */
      buzzer_set_period(666);
      layer4.color = COLOR_RED;
      layer0.color = COLOR_RED;
      layer1.color = COLOR_RED;
    }

    if (!(switches & BIT1)) {
      redrawScreen = 1;
      buzzer_set_period(777);
      layer4.color = COLOR_RED;
      layer0.color = COLOR_RED;
      layer1.color = COLOR_RED;
      layer8.color = COLOR_YELLOW;
      layer3.color = COLOR_YELLOW;
    }

    if (!(switches & BIT2)) {
      redrawScreen = 1;
      buzzer_set_period(888);
      // Color Scheme
      layer7.color = COLOR_GREEN;
      layer6.color = COLOR_GREEN;
      layer5.color = COLOR_RED;
      layer4.color = COLOR_RED;
      layer0.color = COLOR_YELLOW;
      layer1.color = COLOR_YELLOW;
    }

    if (!(switches & BIT3)) {
      redrawScreen = 0;
      clearScreen(COLOR_WHITE);
      drawString8x12(20,40,"This is arch",COLOR_RED, COLOR_WHITE);
      drawString5x7(20,60,"you like that?",COLOR_BLACK, COLOR_WHITE);
      buzzer_set_period(0);
    }  
    count = 0;
  }
  P1OUT &= ~GREEN_LED;    /**< Green LED off when cpu off */
}
