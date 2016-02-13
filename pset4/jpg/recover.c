/**
 * recover.c
 *
 * Computer Science 50x15
 * Problem Set 4
 * Alessio Occhipinti
 * github.com/LasaleFamine
 *
 * Recovers JPEGs from a forensic image.
 * 
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdint.h>
 
 #define PHOTOS 16
 #define SIZE 512

int main(int argc, char* argv[])
{
    // Define the buffer size (8bit int with 512byets of size);
    uint8_t buf[SIZE];
    // Init the counter
    int countImages = 0;
    // Init the writing-file to NULL
    FILE *fW = NULL;
    // Init the name of new file with 8 charac.
    char newFileName[8];
    
    // open input file 
    FILE* fR = fopen("card.raw", "r");
    if (fR == NULL)
    {
        printf("Could not open the card");
        return 2;
    }
    
    // Start read line by line the card
    while (fread(buf, SIZE, 1, fR))
    {
        
        // Check if the first four bytes are a JPEG signature
        if (buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff
            && (buf[3] == 0xe0 || buf[3] == 0xe1))
        {
        
            // Close the previous opened jpg                
            if (fW != NULL)
                fclose(fW);
                
            // Write the new name into the var
            sprintf(newFileName, "%03d.jpg", countImages);
            
            // Open and write the JPG
            fW = fopen(newFileName, "w");
                        
            countImages++;
         }    
         
         if (fW != NULL)
            fwrite(buf, SIZE, 1, fW);   
    }
    
    if (fW != NULL)
        fclose(fW);
        
    fclose(fR);
        
    return 0;
    
}
