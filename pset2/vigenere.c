/**
 * vigenere.c
 *
 * Computer Science 50x2015
 * Problem Set 2
 * Step 3
 * Alessio Occhipinti 
 * github.com/LasaleFamine
 *
 * Encrypt a string with Vigenere method
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define LETTERS_MAX 26

void cypher(string toEncrypt, string cypherString);

int main(int argc, string argv[])
{
     
    // Check if the number of arguments passed
    if (argc != 2)
    {
        printf("Error - usage: ./caesar <key>.\n");
        return 1;
    }
    
    // Get string from argument
    string cypherString = argv[1];
    // Check if the string is only alphabetical
    for(int i = 0, n = strlen(cypherString); i < n; i++)
    {
        if (!isalpha(cypherString[i]))
        {
            printf("Error: argument must be alphabetical\n");
            return 1;
        }
    }  
    
    // Get the string (to encryp) from user and check if valid
    string toEncrypt = GetString();
    if (toEncrypt == NULL)
    {
        printf("Error while getString.\n");
        return 1;
    }
    
    // Call the function to encrypt
    cypher(toEncrypt, cypherString);
    return 0;
}


void cypher(string toEncrypt, string cypherString)
{
    int lowerOrUpper;
    int countCypher = strlen(cypherString);
    char cypherChar;
    int indexCypher = 0;
    
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
            
            // Check the upper/lowecase of the cypherString
            if (isupper(cypherString[indexCypher]))
            {
                cypherChar = cypherString[indexCypher] - 'A';
            }
            else if (islower(cypherString[indexCypher]))
            {
                cypherChar = cypherString[indexCypher] - 'a';
            }
            
            // Create the new encrypted letter with the "real" cypher letter    
            int newChar = (toEncrypt[i] + cypherChar) % LETTERS_MAX;
            // Print the new letter changed to ASCII again
            printf("%c", newChar + lowerOrUpper);
            
            // If is not the end of the cypher string
            if (indexCypher < (countCypher - 1))
            {
                indexCypher++;
            }
            // Else restart from the beginnig
            else
            {
                indexCypher = 0;
            }
        } 
        else
        {
            printf("%c", toEncrypt[i]);
        }
    }
    printf("\n"); 

}
