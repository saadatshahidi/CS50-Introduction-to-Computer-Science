/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(void)
{
    BYTE buffer[512];
    char title[12];
    int count = 0;
    int open = 0;
    FILE* infile = fopen("card.raw", "r");
    if (infile == NULL)
    {
        printf("Could not open raw.jpg\n");
        return 2;
    }
    FILE* out=NULL;
    while(1)
    {   
        int ctrl = fread(buffer, sizeof(BYTE), 512, infile);
        if(ctrl < 512)
        {
            break;
        }
        sprintf(title,  "%03d.jpg", count);
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && 
          (buffer[3] == 0xe0 || buffer[3] == 0xe1))    
        {
            if(open == 1)
            {
                fclose(out);
                count++;
                out = fopen(title, "a");
                if (out == NULL)
                {
                    printf("Could not create\n");
                    return 2;
                }
                
             }
             else 
             {
                out = fopen(title, "a");
                if (out == NULL)
                {
                    printf("Could not create\n");
                    return 2;
                }
                // very important
                open = 1;
                
             }
               
           
         } 
             
        // there must be fwrite here
        if (open==1)
        {
             fwrite(buffer, sizeof(BYTE), 512, out);
        }
    }
    
    fclose(out);
    fclose(infile);
 
    return 0;
}
