/**
 * helpers.c
 *
 * Computer Science 50x2015
 * Problem Set 3
 * Alessio Occhipinti 
 * github.com/LasaleFamine
 *
 * Implementation of helper functions
 */
       
#include <cs50.h>

#include "helpers.h"

bool searchImpro(int value, int values[], int n, int start);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    bool pair = true;
    
    // Check if the number is pair
    if (n % 2 != 0)
    {
        n = n - 1;
        pair = false;
    }    

    // Check if the value is in the middle
    if (values[n / 2] == value)
    {
        return true;
    }
    // Check if the value is in the right half
    else if (value > values[n / 2])
    {   
        // If the value is not pair, add 1 to n
        if (pair == false)
        {
            return searchImpro(value, values, n + 1, n / 2);
        }
        else
        {
            return searchImpro(value, values, n, n / 2);
            
        }            
    }    
    // Check if value is in the left half
    else if (value < values[n / 2])
    {
        return searchImpro(value, values, n / 2, 0);            
    }


    return false;
}

// Fuction to make the binary search
bool searchImpro(int value, int values[], int n, int start)
{
    for(int i = start; i < n; i++)
    {
        if (values[i] == value)
        {
            return true;
        }
    }
    
    return false;    
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int minimum;
    bool stop = false;
    int count = 0, i = 0;
    
    while(!stop)
    {
        // Initialize minimum var with actual count (first time = 0)
        minimum = values[count];
        // Start the loop from the actual count (second time will be = 1)
        for(i = count; i < n; i++)
        {
            // If the actual values is less than the last           
            if (values[i] < minimum)
            {
                // Change the minimum with this actual element
                minimum = values[i];
                // Fix the array
                for(int j = i; j >= count; j--)
                {
                    // Put the minimum in the actual position in exam
                    if (j == count)
                    {
                        values[j] = minimum;
                    }
                    else 
                    {
                        values[j] = values[j - 1]; 
                    }                   
                }
            }
        }
        count++;
        // If we have a O(n^2), we are good
        if (count == n)
        {
            stop = true;
        }
        
    }
    
    return;
}

