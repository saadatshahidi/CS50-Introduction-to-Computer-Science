/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"



typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}node;

int count = 0;
node* bucket[26];



int hash_func(char* keyword)
{
    int hash = toupper(keyword[0]) - 'A';
    return hash;
}

int check_func(const char* wd)
{
    char x = tolower(wd[0]);
    int y = x - 'a';
    return y;
}

int my_strcmp(int len, char* low_case, const char* arbitrary_case)
{
    for (int i = 0; i < len; i++)
    {
        if (low_case[i] != arbitrary_case[i] && low_case[i] != (arbitrary_case[i] + 32))
        {
            return 1;
        }
    }
    return 0;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{ 
    for (int i = 0; i < 26; i++)
    {
        bucket[i] = malloc(sizeof(node));
        if (bucket[i] == NULL)
        {
            printf("NULL error");
        }
    }
    FILE* dc = fopen(dictionary,"r");
    if (dc == NULL)
    {
        printf("Dc is NULL");
        return false;
    }  
   
    node* crawler = bucket[0];
    char tmp[LENGTH + 1];
    int prv_key = 0;
    while(EOF != fscanf(dc,"%s",tmp)) 
    {
        count++;
        int key = hash_func(tmp);
        
        if (key != prv_key)
        {
            crawler = bucket[key];
            prv_key = key;
        }
       
        strcpy(crawler->word ,tmp);
        crawler->next = malloc(sizeof(node));
        if (crawler->next == NULL)
        {
            printf("some next is NULL");
            return false;
        }
        crawler = crawler->next;
    }
    crawler->next = NULL;
    fclose(dc);
    return true;    
        
        
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{   
    int key = check_func(word);
    node* crawler = bucket[key];
    int len = strlen(word);
    while(crawler->next != NULL)
    {
        if (len == strlen(crawler->word))
        {
            if (my_strcmp(len,crawler->word,word) == 0)
            {
                return true;
            }
            else
            {
                crawler = crawler->next;
            }
        }
        else
        {
            crawler = crawler->next;
        }
        
    }
    return false;
}


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return count;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{   
    for (int i = 0 ; i < 26; i++)
    {
        node* crawler = bucket[i];
        while(crawler->next != NULL)
        {   
            crawler = crawler->next;
            free(crawler);
        }
        free(bucket[i]);
    }
    return true;
}
