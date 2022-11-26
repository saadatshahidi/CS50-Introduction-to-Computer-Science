#include <stdio.h>
#include <cs50.h>
#include <string.h>
// including neccesary headers..

char upper(char x)
// function prototype for uppercasing letters...
{
    if (x > 96)
    // checking if the letter is lowercase...
    
    {
        x = x - 32
        /*32=(97(a) - 65(A))*/;
    }
    
    return x;
}

int main(void)
{
    string name;
    char chr;
    int len;
    // declaring variables...
    
    name = GetString();
    // getting the name...
    
    len = strlen(name);
    // getting the length...
    
    chr = upper(name[0]);
    // uppercasing the first letter...
    
    printf("%c",chr);
    // printing the first uppercased letter...
    
    for (int i = 1;i < len; i++)
    
    {   
        char test = name[i];
        
        // cheking for a SPACE...
        if (test == ' ')
        {   
            // uppering and printing initial letters...
            chr = upper(name[i + 1]);
            printf("%c",chr);
        }
    }
    
    printf("\n"); 
    // printing new line...      
}
