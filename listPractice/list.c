#include <stdio.h>
#include <string.h>

typedef struct node
{
    char *name;
    struct node *next;
    
}
node;


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Incorrect usage\n");
        return 1;
    }

    char *fileName = argv[1];
    char name[10];

    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Invalid file\n");
        return 2;
    }

    while(fscanf(file, "%s", name) != EOF)
    {
        printf("Hello %s!\n", name);
    }
    return 0;
}