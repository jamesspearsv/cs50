#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#define LENGTH 14

typedef struct node
{
    char data[LENGTH + 1];
    struct node *next;  
} node;

//Function prototypes
node* createNode(char *data, node *newNode);
node* preprendNode(node *head, node* n);
void printList(node *head);
bool searchList (node* head, char *data);
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

    node *head = NULL;
    node *n = NULL;

    char string[LENGTH + 1];

    while(fscanf(file, "%s", string) != EOF)
    {
        n = createNode(string, n);
        head = preprendNode(head, n);
    }

    //printList(head);


    char searchVal[LENGTH + 1];
    char *exitVal = "exit";
    while (strcasecmp(searchVal, exitVal) != 0)
    {
        printf("Type 'exit' to quit\n");
        printf("What word are you looking for: ");
        scanf("%s", searchVal);

        if (strcasecmp(searchVal, exitVal) == 0)
        {
            printf("\nThank you!\n");
            break;
        }
        else if (searchList(head, searchVal) == true)
        {
            printf("%s is in the list!\n", searchVal);
        }
        else
        {
            printf("%s is not in the list.\n", searchVal);
        }

        printf("\n***************\n");
    }
    freeList(head);

    return 0;
}


//Fuction definitions
node* createNode(char* string, node *newNode)
{
    newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL)
    {
        printf("Memory error creating node\n");
        exit(1);
    } 

    strcpy(newNode->data, string);
    newNode->next = NULL;

    return newNode;
}

node* preprendNode(node *head, node* n)
{
    if (head == NULL)
    {
        head = n;
    }
    else 
    {
        n->next = head;
        head = n;
    }

    return head;
}

void printList(node *head)
{
    node *cursor = head;

    printf("***START***\n");
    while(cursor != NULL)
    {
        printf("%s\n", cursor->data);
        cursor = cursor->next;
    }
    printf("***END***\n");
}

bool searchList (node *head, char *string)
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

void freeList(node *head)
{
    while (head != NULL)
    {
        node *temp = head->next;
        free(head);
        head = temp;
    }
}