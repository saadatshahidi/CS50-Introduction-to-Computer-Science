//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// height and width of game's paddle in pixels
#define PAD_H 20
#define PAD_W 100

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 15

// lives
#define LIVES 3

// brick height and width
#define BRK_H 15
#define BRK_W (WIDTH/COLS)-5

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));
    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);
    // instantiate bricks
    initBricks(window);
    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);
    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);
    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    // number of bricks initially
    int bricks = COLS * ROWS;
    // number of lives initially
    int lives = LIVES;
    // number of points initially
    int points = 0;
    /*************************************************************************/
    add(window, ball);
    // keep playing until game over
    
    double velocity_x = drand48() + 1.2;
    double velocity_y = 2.2;
    waitForClick();
    while (lives > 0 && bricks > 0)
    {
        /************************paddle movement******************************/       
        // check for mouse event
        GEvent event = getNextEvent(MOUSE_EVENT);
        // if we heard one
        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // ensure paddle follows top cursor
                double x = getX(event) - getWidth(paddle) / 2;
                double y = HEIGHT - (PAD_H + 5);
                setLocation(paddle, x, y);
            }
        }
        /****************************ball movement***************************/
        move(ball, velocity_x,velocity_y);
        
        if (getX(ball) + 2 * RADIUS >= WIDTH)
        {
            velocity_x = -velocity_x;
        }
        else if (getX(ball) <= 0)
        {
            velocity_x = -velocity_x;
        } 
        
        if (getY(ball) + 2 * RADIUS >= HEIGHT)
        {   
            setLocation(ball, (WIDTH - 2 * RADIUS) / 2,(HEIGHT - 2 * RADIUS) / 2);
            
            lives--;
            waitForClick();
            continue;
            
        }
        else if (getY(ball) <= 0)
        {
            velocity_y = -velocity_y;
        }
        
        GObject object = detectCollision(window,ball);
        if (object != NULL)
        {
            if (strcmp(getType(object), "GRect") == 0)
            {
                velocity_y = -velocity_y;
            }
            if (object != paddle && object != label)
            {
                removeGWindow(window,object);
                bricks--;
            }
        }
        pause(5);
        /********************** points ***********************************/
        points = ((COLS * ROWS) - bricks) * 2;
        updateScoreboard( window, label, points);
        
    }
    
    /*************************************************************************/
    // wait for click before exiting
    if (bricks <= 0)
    {
         waitForClick();
    }
    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
   
    int row_y = 2;
    
    for (int i = 0; i < ROWS; i++)
    {
        int col_x = 10;
        for (int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(col_x, row_y,BRK_W, BRK_H);
            setColor(brick,"red");
            setFilled(brick,true);
            add(window,brick);
            col_x = col_x + BRK_W + 3;
        }
        row_y = row_y + BRK_H + 2;
     }        
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval((WIDTH - 2 * RADIUS) / 2,(HEIGHT -2 * RADIUS) / 2,2 * RADIUS,2 * RADIUS);
    setColor(ball,"green");
    setFilled(ball,true);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect((WIDTH - PAD_W) / 2, HEIGHT - (PAD_H + 5), PAD_W, PAD_H);
    
    setColor(paddle,"blue");
    
    setFilled(paddle,true);
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    /*
    int i = 0;
    string s = NULL;
    sprintf(s, "%i", i);
    setLabel(label, s);
    
    
    return label;
    */
    
    double x_l = (WIDTH - getWidth(label)) / 2;
    double y_l = (HEIGHT - getHeight(label)) / 2;
    setLocation(label, x_l, y_l); 
    add(window, label);
    
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
