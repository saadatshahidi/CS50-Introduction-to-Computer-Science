#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>
// including neccesary headers...

// function prototype and definition...
// this function is for crypting uppercase letters...
char cypherBig(char key, char x)
{   
    if (0 != isupper(key))
    {
        key = key - 65;
    }
    else 
    {
        key = key - 97;
    }
    
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
// this function is for crypting lowercase letters...
char cypherSmall(char key, char x)
{
    if (0 != isupper(key))
    {
        key = key - 65;
    }
    else 
    {
        key = key - 97;
    }    
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
    string key = argv[1];    
    int lenkey = strlen(key);
    
    for(int k=0;k < lenkey;k++)
    {
        // checking if all characters of the key are letters or alphabets...
        if (0 == isalpha(key[k]))
        {
            printf("Retry:\n");
            return 1;
        }
    }    
    string sent;
    sent = GetString();
    int len = strlen(sent);
        
    int j = 0;
    
    // iterating through each characters...
    for(int i = 0; i < len; i++)
    {
        char test = sent[i];
        char key_temp = key[j];       
        
        // checking if the character is an uppercase alphabet...
        if ( 0 != isupper(test))
        {
            test = cypherBig(key_temp,test);
            j++;
            
        }
         // checking if the character is an lowercase alphabet...
        else if (0 != islower(test))
        {   
            test = cypherSmall(key_temp,test);
            j++;
        }
        
        // printing the characters...    
        printf("%c",test); 
        
        if (j == lenkey )
        {
            j = 0;
        }   
     } 
    printf("\n");
}
