#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
// including neccesary headers...

// function prototype and definition...
char cryptoBig(char x, int key)
{
    if (x > 64 && x <= (90 - key)) 
    {   
        x = x + key;
    }
    else
    {
        x = x - 26 + key;
    }
    
    return x;
}

// function prototype and definition...
char cryptoSmall(char x, int key)
{
    if (x > 96 && x <= (122 - key)) 
    {   
        x = x + key;
    }
    else
    {
        x = x - 26 + key;
    }
    
    return x;
}

int main(int argc, string argv[])
{   
    // handling lack of argv[1]...    
    if (argc != 2)
    {
        printf("Retry:\n");
        return 1;
    }
    
    int key, len;
    string sent;
       
    
    key = atoi(argv[1]);
    key = key % 26;
    sent = GetString();
    len = strlen(sent);
    
    // iterating through each characters...
    for(int i = 0; i < len; i++)
    {
        char test = sent[i];
        
        // checking if the character is an uppercase alphabet...
        if ( (test > 64) && (test < 91))
        {
            test = cryptoBig(test,key);
        }
         // checking if the character is an lowercase alphabet...
        else if ((test > 96) && (test < 123))
        {   
            test = cryptoSmall(test,key);
        }
        
        // printing the characters...        
        printf("%c",test); 
       
        
     } 
     
    printf("\n");
}
