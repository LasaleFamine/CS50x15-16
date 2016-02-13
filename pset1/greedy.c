/**
 * greedy.c
 *
 * Computer Science 50x2015
 * Problem Set 1
 * Alessio Occhipinti 
 * github.com/LasaleFamine
 *
 * Greedy count.
 */

#include <cs50.h>
#include <math.h>
#include <stdio.h>

int getRest(int value, int divider, int rest);
int division(int value, int divider, int rest);

int main(void)
{
    const int coins[4] = {25, 10, 5, 1};
    int countCoins[4];
    float val;
    int converted;
    bool passOver = false;
    int rest = -2;
    int i = 0, countTotal = 0;
    
    // Start first loop
    do
    {
        // Ask and get the value
        printf("Hi, how much change is owed?\n");
        val = GetFloat();
        // Check if value is valid
        if (val <= 0) 
        {
            printf("Negative value or not valid\n");
        }
        else
        {
            // Covert and round the value to cent and int
            converted = (int) (round(val * 100));   
            passOver = true;                              
        }   
    }
    while(passOver == false);

    // Initialize the count
    for(i = 0; i < 4; i++)
    {   
        // Make the first division
        countCoins[i] = division(converted, coins[i], rest); 
        // Start the count   
        countTotal += countCoins[i]; 
                                  
        if (countCoins[i] != 0)                                 
        {
            // Check for remainder
            rest = getRest(converted, coins[i], rest); 
        }
        // If remainder is 0, work done
        if (rest == 0)                                          
        {  
            // Print the total count and exit
            printf("%d\n", countTotal);            
            return 0;                                        
        } 
              
    }
            
}


// Function to make the division
int division(int value, int divider, int rest)
{
    int result;

    if (rest != 0 && rest != -2) 
    {
       // If remainder is avail  
        result = rest / divider;        
    }
    else if (rest == -1)
    {
        // If rem is not valid
        return -1;                 
    }
    else
    {
        // If rem is not avail
        result = value / divider; 
    }   
    
    return result;
    
}

// Function to get the remainder
int getRest(int value, int divider, int rest)
{
    int try;
    
    if (rest != 0 && rest != -2) 
    {
        try = rest % divider;        
    }
    else
    {
        try = value % divider;
    }
    // If the division have a valid remainder        
    if (try != divider)     
    {
        // Return the value of remainder
        return try;         
    }
    else
    {   
        // Return -1 to indicate "impossible to divide"
        return -1;          
    } 
    
}
