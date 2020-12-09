#include <stdio.h>
#include <cs50.h>
#include <math.h>


void coinNumber(int x);

int main(void)
{
float change;
do
{
   change = get_float("Change owed: ");
}
while (change < 0);
int changeInt = round(change * 100);
coinNumber(changeInt);
}


void coinNumber(int x)
{
int n = 0; //number of coins used
    while (x >= 25) {
        x = x - 25;
        n++;
    }
     while (x >= 10) {
        x = x - 10;
        n++;
    }

     while (x >= 5) {
        x = x - 5;
        n++;
    }

     while (x >= 1) {
        x = x - 1;
        n++;
    }
printf("%i\n", n);
}