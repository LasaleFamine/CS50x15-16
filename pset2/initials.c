/**
 * initials.c
 *
 * Computer Science 50x2015
 * Problem Set 2
 * Step 1
 * Alessio Occhipinti
 * github.com/LasaleFamine
 *
 * Get initials from a string given.
 */

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Define functions
int stringValid(string a_string);
void getInitial(string a_string);

int main(void)
{
    string name;
    
    // Get the string while isValid
    do
    {
        name = GetString();
    }
    while(stringValid(name) != 0);
    
    // If something was wrong with GetString()
    if (name == NULL)
    {
        printf("Error getString\n");
        return -1;
    }
    
    // Get the initials and print them
    getInitial(name);  
}

// Fucntion to check the string
int stringValid(string a_string)
{
    int i, n;
    for(i = 0, n = strlen(a_string); i < n; i++)
    {
        // If is not a letter or a space
        if (!isalpha(a_string[i]) && !isspace(a_string[i]))
        {
            printf("Error.\n");
            return -1;
        }
    }
    return 0;    
}

// Function to get the initials and print
void getInitial(string a_string)
{
    int i, n;
    
    for(i = 0, n = strlen(a_string); i < n; i++)
    {
        if (i == 0 || isspace(a_string[i - 1]))
        {
            // Upper case the letter before print
            printf("%c", toupper(a_string[i]));
        }
    }
    printf("\n");
}
