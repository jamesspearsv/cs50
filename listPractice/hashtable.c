#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#define LENGTH 15 //Max word length
#define TABLESIZE 31 //Table length

typedef struct node
{
    char data[LENGTH + 1];
    struct node *next;  
} node;

//Function prototypes
int hash(char *string);
node* createNode(char *data, node *newNode);
node* preprendNode(node *head, node* n);
void printList(node *head);
bool searchTable (node* head, char *data);
void freeList(node *head);

int main(int argc, char *argv[])
{
   if (argc != 2)
    {
        printf("Incorrect usage\n");
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Invalid file error: 2\n");
        return 2;
    }

    //Create table and head prointers
    node *hashTable[TABLESIZE];
    for (int i = 0; i < TABLESIZE; i++)
    {
        //Set head pointer to NULL for each table index
        hashTable[i] = NULL;
    }
    node *n = NULL; //Temp pointer
    

    //Read data from infile and create table
    char string[LENGTH + 1];
    while(fscanf(file, "%s", string) != EOF)
    {
        int hashKey = hash(string);
        n = createNode(string, n);
        hashTable[hashKey] = preprendNode(hashTable[hashKey], n);
    }
    fclose(file);

    /*printf("***START***\n");
    for (int i = 0; i < TABLESIZE; i++)
    {
        printf("\n\n***INDEX %i***\n", i);
        printList(hashTable[i]);
    }
    printf("***END***"); */

    //Search table with user input
    char searchVal[LENGTH + 1];
    char *exitVal = "/exit";
    while (strcasecmp(searchVal, exitVal) != 0)
    {
        printf("Type '/exit' to quit\n");
        printf("What word are you looking for: ");
        scanf("%s", searchVal);
        int searchKey = hash(searchVal);

        if (strcmp(searchVal, exitVal) == 0)
        {
            printf("\nThank you!\n");
            break;
        }
        else if (searchTable(hashTable[searchKey], searchVal) == true)
        {
            printf("%s is in the list!\n", searchVal);
        }
        else
        {
            printf("%s is not in the list.\n", searchVal);
        }

        printf("\n***************\n");
    }

    //Free list at each table index
    for (int i = 0; i < TABLESIZE; i++)
    {
        freeList(hashTable[i]);
    }
    return 0;
}


//Fuction definitions
int hash(char *string)
{
    int sum = 0;
    int n = strlen(string);

    for (int i = 0; i < n; i++)
    {
        int tmp = tolower(string[i]);
        sum = sum + (tmp * tmp);
    }
    sum = sum % TABLESIZE;
    return sum;
}

node* createNode(char* string, node *newNode)
{
    newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL)
    {
        printf("Memory error creating node\n");
        exit(3);
    } 

    strcpy(newNode->data, string);
    newNode->next = NULL;

    return newNode;
}

//Push new node to front of list
node* preprendNode(node *head, node* nodeToPrepend)
{
    if (head == NULL)
    {
        head = nodeToPrepend;
    }
    else 
    {
        nodeToPrepend->next = head;
        head = nodeToPrepend;
    }

    return head;
}

//Prints linked list with given head pointePre
void printList(node *head)
{
    node *cursor = head;
    if (cursor == NULL) printf("NULL\n");

    //printf("***START***\n");
    while(cursor != NULL)
    {
        printf("%s\n", cursor->data);
        cursor = cursor->next;
    }
    //printf("***END***\n");
}

//Searched linked like with given head pointer and search value
bool searchTable (node *head, char *string)
{
    node *cursor = head;
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->data, string) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

//Frees list with given head pointer
void freeList(node *head)
{
    while (head != NULL)
    {
        node *temp = head->next;
        free(head);
        head = temp;
    }
}