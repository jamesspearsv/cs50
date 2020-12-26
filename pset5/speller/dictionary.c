// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
//Better to use prime numbers for hash function distribution
//large table sizes also decrease search time
const unsigned int N = 49999;

// Hash table
node *table[N];

//number of words in infile
int numberOfWords = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //Set cursor node to list head at key index
    node *cursor = table[hash(word)];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    /*
    *** I initially wrote this simple ***
    *** hash function to hash strings ***
    int sum = 0;
    int len = strlen(word);

    for (int i = 0; i < len; i++)
    {
        int tmp = tolower(word[i]);
        sum = sum + (tmp * tmp);
    }
    sum = sum % (N - 1);
    return sum;
    */

    //  This sdbm hash function was shared by
    //  Mecki on stackoverflow @
    //  https://stackoverflow.com/questions/14409466/simple-hash-functions
    //  and adapted to work in this program.

    int len = strlen(word);
    int hashAddress = 0;
    for (int counter = 0; counter < len; counter++)
    {
        hashAddress = tolower(word[counter]) + (hashAddress << 6) + (hashAddress << 16) - hashAddress;
    }
    hashAddress = hashAddress % N;
    return hashAddress;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL) return false;

    //Set head at each table index to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    //temporary node to link lists
    node *temp;

    //Reads strings from file and places in table
    char string[LENGTH + 1];
    while (fscanf(file, "%s", string) != EOF)
    {
        //Allocate memory for new node stored in temp
        temp = (node*)malloc(sizeof(node));
        if (temp == NULL) return false;

        int hashKey = hash(string);
        numberOfWords++;

        //Copy data to new node and sets next pointer to NULL
        strcpy(temp->word, string);
        temp->next = NULL;

        //Pushes node to correct index in table
        if (table[hashKey] == NULL) //List is empty
        {
            table[hashKey] = temp;
        }
        else //List is not empty
        {
            temp->next = table[hashKey];
            table[hashKey] = temp;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return numberOfWords;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *temp = table[i]->next;
            free(table[i]);
            table[i] = temp;
        }
    }
    return true;
}