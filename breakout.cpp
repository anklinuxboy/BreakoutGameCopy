
// standard libraries
#define _XOPEN_SOURCE
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// Stanford Portable Library
#include "StanfordCPPLib/gevents.h"
#include "StanfordCPPLib/gobjects.h"
#include "StanfordCPPLib/gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// DIAMETER of ball in pixels
#define DIAMETER 10

// lives
#define LIVES 3

//height and width of paddle
#define PADDLE_HEIGHT 10
#define PADDLE_WIDTH 40

//height and width of bricks
#define BRICKS_HEIGHT 15
#define BRICKS_WIDTH 37
#define TOP_HEIGHT 40

// prototypes

void initBricks(GWindow window);
//GOval initBall(GWindow window);
GObject* detectCollision(GWindow window, GOval* ball);
void updateScoreboard(GWindow window, GLabel* label, int points);
void updatelives(GWindow window, GLabel* label, int lives);

//x and y speeds for ball
double xvel = 2.0;
double yvel = 2.0;

int main()
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = GWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);
   
    // instantiate ball, centered in middle of window
    GOval* ball = new GOval(195, 200, DIAMETER, DIAMETER);

    //set color
    ball->setColor("BLACK");
    ball->setFilled(true);

    //add ball to window
    window.add(ball);
     
    // instantiate paddle, centered at bottom of window
    GRect* paddle = new GRect(180, 550, PADDLE_WIDTH, PADDLE_HEIGHT);
    //fill color
    paddle->setColor("BLACK");
    paddle->setFilled(true);

    //add paddle to window
    window.add(paddle);

    //print score on screen
    GLabel* score = new GLabel("Score");

    //set font to label
    score->setFont("SansSerif-15");
    double xsc = 340;
    double ysc = 260;
    //add label to window
    window.add(score, xsc, ysc);


    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel* label = new GLabel("0");

    //set font to label
    label->setFont("SansSerif-15");
    double x1 = 360;
    double y1 = 300;
    //add label to window
    window.add(label, x1, y1);
   
    //lives on screen
    GLabel* l = new GLabel("Lives");
    //set font to label
    l->setFont("SansSerif-15");
    double xl = 10;
    double yl = 260;
    //add label to window
    window.add(l, xl, yl);
    
    //label for lives
    GLabel* live = new GLabel("3");
    live->setFont("SansSerif-15");
    double xco = 20;
    double yco = 300;
    window.add(live, xco, yco);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    GLabel* start = new GLabel("Click to Start!");

    //set font to label
    start->setFont("SansSerif-20");
    double x2 = 130;
    double y2 = 400;
    start->setColor("GREEN");
    window.add(start, x2, y2);
    //wait for click before starting
    waitForClick();
    pause(500);
    window.remove(start);
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        GMouseEvent event = getNextEvent(MOUSE_EVENT);
        if (event.isValid())
        {         
            // if the event was movement
            if (event.getEventType() == MOUSE_MOVED)
            {
                // ensure circle follows top cursor
                double x = event.getX() - paddle->getWidth() / 2;
                double y = 550;
                if (x <= 1)
                {
                    paddle->setLocation(1, y);
                }
                else
                {
                    paddle->setLocation(x, y);
                }
            }
        }

        updatelives(window, live, lives);
        //bounce off the edges
        ball->move(xvel, yvel);
        //detect collision
        GObject* object = detectCollision(window, ball);
        if (object != NULL)
        {
            if (object->getType() == "GRect")
            {
                if (object == paddle)
                {
                    yvel = -yvel;
                }
                else
                {
                    window.remove(object);
                    yvel = -yvel;
                    yvel += 0.1;
                    xvel += 0.1;
                    points++;
                    bricks--;
                    updateScoreboard(window, label, points);
                }
            }            
        }

        //bounce off right edge of window
        if (ball->getX() + ball->getWidth() >= window.getWidth())
        {
            xvel = -xvel;
        }
        //bounce off left edge 
        else if (ball->getX() + xvel <= 0)
        {
            xvel = -xvel;
        }
        //bounce off top edge
        else if (ball->getY() + yvel <= 0)
        {
            yvel = -yvel;
        }
        //check bottom edge
        else if (ball->getY() + ball->getHeight() >= window.getHeight())
        {
            lives -= 1;
            window.remove(ball);
            pause(1000);
            ball = new GOval(195, 200, DIAMETER, DIAMETER);
            ball->setColor("BLACK");
            ball->setFilled(true);
            window.add(ball); 
            pause(1000);           
        }
        pause(10);

        if (bricks == 0)
        {
            GLabel* win = new GLabel("Congrats!!You won!!!");
            //set font to label
            win->setFont("SansSerif-25");
            double x3 = 100;
            double y3 = 150;
            win->setColor("BLUE");
            window.add(win, x3, y3);
            pause(500);
            window.remove(win);

            GLabel* end = new GLabel("Click to Exit");
            //set font to label
            end->setFont("SansSerif-25");
            double x4 = 100;
            double y4 = 500;
            end->setColor("BLUE");
            window.add(end, x4, y4);
            break;
        }
        if (lives == 0)
        {
            GLabel* lose = new GLabel("Better Luck Next Time");
            //set font to label
            lose->setFont("SansSerif-20");
            double x3 = 100;
            double y3 = 500;
            lose->setColor("RED");
            window.add(lose, x3, y3);
            pause(700);
            window.remove(lose);
            break;
        }
    }
    // wait for click before exiting
    waitForClick();

    // game over
    window.close();
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int i;
    int top_height = TOP_HEIGHT;
    for (i = 0; i < ROWS; i++)
    {
        int SIDE_WIDTH = 3;
        for (int j = 0; j < COLS; j++)
        {
            GRect* brick = new GRect(SIDE_WIDTH, top_height, BRICKS_WIDTH, BRICKS_HEIGHT);
            SIDE_WIDTH += BRICKS_WIDTH + 2;
            switch(i) 
            {
                case 0:
                {
                    brick->setFillColor("GREEN");
                    break;
                }
                case 1:
                {
                    brick->setFillColor("RED");
                    break;
                }
                case 2:
                {
                    brick->setFillColor("YELLOW");
                    break;
                }
                case 3:
                {
                    brick->setFillColor("ORANGE");
                    break;
                }
                case 4:
                {
                    brick->setFillColor("BLUE");
                    break;
                }                
            }
            brick->setFilled(true);
            window.add(brick);
        }
        top_height += BRICKS_HEIGHT + 10;
    }
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel* label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    label->setLabel(s);

    // center label in window
    double x = 360;
    double y = 300;
    label->setLocation(x, y);
}

void updatelives(GWindow window, GLabel* label, int lives)
{
   // update label
    char s[12];
    sprintf(s, "%i", lives);
    label->setLabel(s);

    // center label in window
    double x = 20;
    double y = 300;
    label->setLocation(x, y);   
}

/*
 * Detects collision of the ball
*/
GObject* detectCollision(GWindow window, GOval* ball)
{
    // ball's location
    double x = ball->getX();
    double y = ball->getY();

    // for checking for collisions
    GObject* object;

    // check for collision at ball's top-left corner
    object = window.getGObjectAt(x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = window.getGObjectAt(x + DIAMETER, y);
    if (object != NULL)
    {        
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = window.getGObjectAt(x, y + DIAMETER);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = window.getGObjectAt(x + DIAMETER, y + DIAMETER);
    if (object != NULL)
    {
        return object;
    }
    // no collision
    return NULL;
}