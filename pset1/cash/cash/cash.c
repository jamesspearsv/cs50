#include <stdio.h>
#include <cs50.h>
#include <math.h>


void coinNumber(int x);

int main(void)
{
float change;
//Prompts user for input and checks for positive number
do
{
    change = get_float("Change owed: ");
}
while (change < 0);
//Converts float to interger and removes decimal
int changeInt = round(change * 100);
coinNumber(changeInt);
}



void coinNumber(int x)
{
int n = 0; //number of coins used
    //Checks if quarters can be used
    while (x >= 25)
    {
        x = x - 25;
        n++;
    }
    //Checks if dimes can be used
    while (x >= 10)
    {
        x = x - 10;
        n++;
    }
    //Checks if nickles can be used
    while (x >= 5)
    {
        x = x - 5;
        n++;
    }
    //Checks if pennies can be used
    while (x >= 1)
    {
        x = x - 1;
        n++;
    }
//Prints output
printf("%i\n", n);
}