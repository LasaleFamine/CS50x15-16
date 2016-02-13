/**
 * caesar.c
 *
 * Computer Science 50x2015
 * Problem Set 2
 * Step 2
 * Alessio Occhipinti 
 * github.com/LasaleFamine
 *
 * Encrypt a string with Caesar method.
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define LETTERS_MAX 26

void cypher(string toEncrypt, int cypherN);

int main(int argc, string argv[])
{

    int cypherNumber = 0;  
     
    // Check if the number of arguments passed
    if (argc != 2)
    {
        printf("Error: arguments must be 2 (including the program name): ./caesar <key>.\n");
        return 1;
    }   
    
    // Convert to an int the passed value
    cypherNumber = atoi(argv[1]);    
    
    // If the conversion can't be done
    if (cypherNumber == 0)
    {
        printf("The argument is not a number\n");
        return 1;
    }
    
    // Check if the number is positive
    else if (cypherNumber < 0)
    {
        printf("Error: the argument must be a non-negative int\n");
        return 1;
    }
    
    // Get the string from user and check if valid
    string toEncrypt = GetString();
    if (toEncrypt == NULL)
    {
        printf("Error while getString.\n");
        return 1;
    }
    
    // Call the function to encrypt
    cypher(toEncrypt, cypherNumber);
    return 0;
}


void cypher(string toEncrypt, int cypherN)
{
    int lowerOrUpper;
    
    // Start the loop until the last letter of the input string
    for(int i = 0, n = strlen(toEncrypt); i < n; i++)
    {   
        // If is a real letter (not symbol or number)     
        if (isalpha(toEncrypt[i]))
        {
            // If is an uppercase letter
            if (isupper(toEncrypt[i]))
            {
                lowerOrUpper = 'A';
            }
            
            // If is a lowercase letter
            else if (islower(toEncrypt[i]))
            {
                lowerOrUpper = 'a';
            }
            
            // Change the ASCII value to a real alphabet value
            toEncrypt[i] = toEncrypt[i] - lowerOrUpper;  
            // Create the new encrypted letter    
            int newChar = (toEncrypt[i] + cypherN) % LETTERS_MAX;
            // Print the new letter changed to ASCII again
            printf("%c", newChar + lowerOrUpper);
        } 
        else
        {
            printf("%c", toEncrypt[i]);
        }
    }
    printf("\n"); 

}
