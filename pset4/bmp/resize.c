/**
 * resize.c
 *
 * Computer Science 50x15
 * Problem Set 4
 * Alessio Occhipinti
 * github.com/LasaleFamine
 * 
 * Resize a BMP file
 * Usage: ./resize n infile outfile
 *
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
   
    int nRes = atoi(argv[1]);
    
    if (nRes < 1 || nRes > 100)
    {
        printf("The factor 'n' must be an integer > 1 and < 100");
        return 1;    
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bf_res;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bf_res = bf;
    

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi_res;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bi_res = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // Get and upgrade old Width and Height
    bi_res.biWidth = bi.biWidth * nRes;
    bi_res.biHeight = bi.biHeight * nRes;

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_res =  (4 - (bi_res.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi_res.biSizeImage = (bi_res.biWidth * sizeof(RGBTRIPLE) + padding_res) * abs(bi_res.biHeight);
    bf_res.bfSize = bf.bfSize - bi.biSizeImage + bi_res.biSizeImage;
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_res, sizeof(BITMAPFILEHEADER), 1, outptr);
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_res, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for (int l = 0; l < nRes; l++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);               
                
                // write RGB triple to outfile
                for (int z = 0; z < nRes; z++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }                    
            }
            
            // Add the new padding
            for (int z = 0; z < padding_res; z++)
            {
                fputc(0x00, outptr);
            }
            
            // Return at the start of the scanline
            if (l < nRes - 1)
            {             
                fseek(inptr, -bi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
            }          
        }
        
        // Skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);       
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
