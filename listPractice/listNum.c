#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node
{
    int data;
    struct node *next;  
} node;

//Function prototypes
node* createNode(int val, node *newNode);
node* preprendNode(node *head, node* n);
void printList(node *head);
bool searchList (node* head, int val);
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

    int val;

    while(fscanf(file, "%i", &val) != EOF)
    {
        n = createNode(val, n);
        head = preprendNode(head, n);
    }

    int searchVal = 0;
    int exitVal = -1;
    while (searchVal != exitVal)
    {
        printf("Which numer are you looking for?\nType '-1' to quit\n");
        scanf("%i", &searchVal);
    
        if (searchVal == exitVal) 
        {
            printf("Thank you!\n");
            break;
        }
        else if (searchList(head, searchVal))
        {
            printf("%i is in the list!\n", searchVal);
        }
        else
        {
            printf("%i is not in the list.\n", searchVal);
        }

        printf("**********************************************\n");
        printf("**********************************************\n");
    }

    freeList(head);

    return 0;
}


//Fuction definitions
node* createNode(int val, node *newNode)
{
    newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL)
    {
        printf("Memory error creating node\n");
        exit(1);
    } 

    newNode->data = val;
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
        printf("%i\n", cursor->data);
        cursor = cursor->next;
    }
    printf("\n***END***\n");
}

bool searchList (node* head, int val)
{
    node* cursor = head;
    while (cursor != NULL)
    {
        if (cursor->data == val)
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