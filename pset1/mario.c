/**
 * mario.c
 *
 * Computer Science 50x2015
 * Problem Set 1
 * Alessio Occhipinti 
 * github.com/LasaleFamine
 *
 * Mario pyramid.
 */

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    bool startDraw = false;
    int i, j;
    
    // Start the loop
    do
    {
        // Get int from user
        printf("Height of pyramid: ");
        height = GetInt();
        
        // Check for valid int
        if (height < 0)
        {
            printf("Negative int is not permitted\n");
        }
        else if (height > 23)
        {
            printf("Too large\n");
        }
        else if (height >= 0 && height <= 23)
        {
            // Putting this on "true" state, this loop will finish
            startDraw = true;
        }
        else
        {
            // Everything is not in the gap above is not recognized
            printf("Can't recognize valid int\n");
        }
    
    }
    while(startDraw == false);
    // Start drawing
    for(i = 0; i < height; i++)
    {
        for(j = 0; j <= height; j++)
        {
            if (j > (height - i - 2))
            {
                // If in the right point on the right
                printf("#");
            }
            else 
            {
                // Try to get the right point
                printf(" ");
            }
            
        }
        // To change row after drawing 
        printf("\n");        
    }
}
