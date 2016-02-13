/**
 * dictionary.c
 *
 * Computer Science 50x16
 * Problem Set 5
 * Alessio Occhipinti 
 * github.com/LasaleFamine
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Tracks the number of entries in the dictionary
unsigned int dictionarySize = 0;
// Root node
node* root;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int wordLength = strlen(word);
    
    // Poiting to the current node through the trie
    node* ptr = root;
    
    // Check the word char by char
    for (int i = 0; i < wordLength; i++)
    {
        char c = tolower(word[i]);
        // Check for the apostrophe
        if (c == '\'')
        {
            // Aphostrophe go to nodes[26].
            c = APHOSTROPHE;
        }
        
        // Try get the addr of the next node
        struct node* nextNode = ptr->nodes[c - ASCII_DIFF];
        if (nextNode == NULL)
        {
            return false;
        }
        else 
        {
            ptr = nextNode;
        }
    }
    
    // Check if the words is really in dict
    if (ptr->exists == true)
    {
        return true;
    }
    
    return false;
    
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Open the input file
    FILE* dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    
    // Push and allocate space for first node
    root = calloc(1, sizeof(node));
    
    // And point to the current node declaring ptr(pointer)
    node* ptr = root;
    
    // For each char (lowercase) in "dict"
    // while char is not EOF
    // get a char from the dict
    for(char c = tolower(fgetc(dict)); c != EOF; c = fgetc(dict))
    {
        // Check is the word is finished
        if (c == '\n')
        {
            // Check if the word is at least on letter
            if (ptr != root)
            {
                dictionarySize++;
                // Set the word find in struct
                ptr->exists = true;
                // Reset the pointer
                ptr = root;
            }
        }
        
        // Else if we are checking a word
        else
        {
            // Check for the apostrophe
            if (c == '\'')
            {
                // Aphostrophe go to nodes[26].
                c = APHOSTROPHE;
                
            }
            
            // Try to get the new next node addr
            struct node* nextNode = ptr->nodes[c - ASCII_DIFF];
            if (nextNode == NULL)
            {
                // We need to allocate the new space
                ptr->nodes[c - ASCII_DIFF] = calloc(1, sizeof(node));
                ptr = ptr->nodes[c - ASCII_DIFF];
            }
            else 
            {
                ptr = nextNode;
            }
        }
    
    }
    
    // Now we can close the dict file
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return dictionarySize;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    memCleaner(root);
    return true;
}



/**
 * Utils
 * Function to cleaer a trie from memory
 */
 
 void memCleaner(node* pointer)
 {
     for(int i = 0; i < ALPHABET_SIZE + 1; i++)
     {
         if(pointer->nodes[i] != NULL)
         {
             memCleaner(pointer->nodes[i]);
         }
     }
     
     free(pointer);
 }
