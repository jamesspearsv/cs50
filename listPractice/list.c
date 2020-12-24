#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Max word length
const int LENGTH = 11;

typedef struct node
{
    char name[LENGTH];
    struct node *next;  
}
node;

//Function prototypes



int main(int argc, char *argv[])
{
   if (argc != 2)
    {
        printf("Incorrect usage\n");
        return 1;
    }

    char *fileName = argv[1];
    char name[LENGTH];

    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Invalid file error: 2\n");
        return 2;
    }

    //Head for linked list
    node *head;
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        printf("Memory error: 3\n");
        return 3;
    }

    while(fscanf(file, "%s", name) != EOF)
    {   
        //Copy value into node
        strcpy(n->name, name);
        n->next = NULL;

        //Set next pointer
        if (head == NULL) 
        {
            head = n;
        }
        else
        {
            n = head;
            head = n;
        }
    }

    node *tmp = head;
    while (tmp != NULL)
    {
        printf("Hello %s\n", tmp->name);
        tmp = tmp->next;
    }
    free(n);
    return 0;
}