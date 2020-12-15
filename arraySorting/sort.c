#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>

int main(void)
{

    typedef struct
    {
        string name;
        int votes;
    }
    record;

    record array[3];
    array[0].name = "James";
    array[1].name = "David";
    array[2].name = "Sarah";

    array[0].votes = 7;
    array[1].votes = 1;
    array[2].votes = 5;

    int tempNum, n = 3;
    string tempName;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (array[i].votes < array[j].votes)
            {
                //Move number into place
                tempNum = array[j].votes;
                array[j].votes = array[i].votes;
                array[i].votes = tempNum;

                //Move name into place
                tempName = array[j].name;
                array[j].name = array[i].name;
                array[i].name = tempName;
            }
        }
    }

    for (int a = 0; a < n; a++)
    {
        printf("%i ", array[a].votes);
        printf("%s ", array[a].name);
    }
    printf("\n");
}