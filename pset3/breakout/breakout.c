/**
 * breakout.c
 *
 * Computer Science 50x2015
 * Problem Set 3
 * Alessio Occhipinti 
 * github.com/LasaleFamine
 *
 * Little game Breakout
 */

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

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 20

// lives
#define LIVES 3

// Paddle definition
#define PADDLE_WIDTH 50
#define PADDLE_HEIGHT 10

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
    
    // initial velocity
    double standardReduction = 0.07;
    double velocity = drand48() - standardReduction;
    double velocityY = 2;

    waitForClick();
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // Get the next mouse event
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        // If some move
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - (PADDLE_WIDTH / 2);
                double y = 500;
                setLocation(paddle, x, y);
            }
        }
                
        move(ball, velocity, velocityY);
        
        // bounce off right edge of window
        if (getX(ball) + RADIUS >= WIDTH)
        {
            velocity = -velocity;
        }
        // bounce off left edge of window
        else if (getX(ball) <= 0)
        {
            velocity = -velocity;
        }
        // bounce off top edge
        else if (getY(ball) <= 0)
        {
            velocityY = -velocityY;
        }
        // bottom edge
        else if (getY(ball) + RADIUS >= HEIGHT)
        {
            velocityY = 0;
            velocity = 0;
            lives--;
            removeGWindow(window, ball);
            removeGWindow(window, paddle);
            ball = initBall(window);
            paddle = initPaddle(window);
            velocity = drand48() - standardReduction;
            velocityY = 2;
            waitForClick();
        }

        GObject obj = detectCollision(window, ball);
        if (obj != NULL)
        {
            if (obj == paddle)
            {
                velocityY = -velocityY;
            }
            else if (strcmp(getType(obj), "GRect") == 0)
            {
                removeGWindow(window, obj);
                bricks--;
                points++;
                updateScoreboard(window, label, points);
                velocityY = -velocityY;
            }
        }
        
        pause(3);       
    }

    if (bricks == 0)
    {
        updateScoreboard(window, label, 51);
    }
    else 
    {
        updateScoreboard(window, label, 0);
    }
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int x = 2;
    int y = 20;
    string colors[] = {"MAGENTA", "ORANGE", "YELLOW", "RED", "BLUE"};
    
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {              
            GRect rect = newGRect(x, y, 35, 10); 
            setFilled(rect, true);
            setColor(rect, colors[i]);
            add(window, rect);
            x += 40;    
        } 
        x = 2;       
        y += 14;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    int posX = (WIDTH / 2) - RADIUS;
    int posY = (HEIGHT / 2) - 80;  
    GOval circle = newGOval(posX, posY, RADIUS, RADIUS);
    setFilled(circle, true);
    setColor(circle, "DARK_GRAY");
    add(window, circle);
    return circle;  
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{

    int posX = (WIDTH / 2) - 65 / 2;
    int posY = 500;  
    GRect rect = newGRect(posX, posY, 65, 12);
    setFilled(rect, true);
    setColor(rect, "DARK_GRAY");
    add(window, rect);
    return rect;    
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    int posX = (WIDTH / 2) - 19;
    int posY = (HEIGHT / 2) - 19;
    GLabel label = newGLabel("0");
    setFont(label, "Ubuntu-bold-30");
    setColor(label, "LIGHT_GRAY");
    setLocation(label, posX, posY);
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
    if (points == 51)
    {
        setLabel(label, "WIN!");
    }
    else if (points == 0)
    {
        setLabel(label, ":(");
    }
    else 
    {
        setLabel(label, s);
    }
    
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
    object = getGObjectAt(window, x + 1 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 1 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 1 * RADIUS, y + 1 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
