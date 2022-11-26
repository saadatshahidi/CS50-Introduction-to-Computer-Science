/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Resizes a BMP piece by piece, just because.
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
    int n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];
    
    if ( n < 1 || n > 100)
    {
        printf ("n=[0,100]\n");
        return 5;
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
    BITMAPFILEHEADER ibf;
    fread(&ibf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER ibi;
    fread(&ibi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (ibf.bfType != 0x4d42 || ibf.bfOffBits != 54 || ibi.biSize != 40 || 
        ibi.biBitCount != 24 || ibi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    BITMAPFILEHEADER obf;
    obf = ibf;
    
    BITMAPINFOHEADER obi;
    obi = ibi;

    // updating a part of header    
    obi.biWidth = (ibi.biWidth) * n;
    obi.biHeight = (ibi.biHeight) * n;
    
    
    // determine padding for scanlines
    int i_padding =  (4 - (ibi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int o_padding =  (4 - (obi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // updating the rest part of header
    obi.biSizeImage = (obi.biWidth * sizeof(RGBTRIPLE) + o_padding) * abs(obi.biHeight);
    obf.bfSize = obi.biSizeImage + obf.bfOffBits;
     
    // write outfile's BITMAPFILEHEADER
    fwrite(&obf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&obi, sizeof(BITMAPINFOHEADER), 1, outptr);  
   
    // iterate through lines
    for ( int i = 0, biHeight = abs(ibi.biHeight); i < biHeight; i++)
    {            
        RGBTRIPLE trpls[ibi.biWidth];
        fread(&trpls, sizeof(RGBTRIPLE), ibi.biWidth, inptr);
        for (int j = 0; j < n;j++)
        {
            for (int k = 0; k < biHeight; k++)
            {
                for (int l = 0; l < n; l++)
                {
                    fwrite(&trpls[k], sizeof(RGBTRIPLE), 1 , outptr);
                 }
            }
                
            for (int m = 0; m < o_padding; m++)
            {
                fputc(0x00, outptr);
            }
         }
             
        fseek(inptr,i_padding, SEEK_CUR);
            
        
     }
             

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
