#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
float change;
int changeInt;
do
{
   change = get_float("How much changes is owed: ");
}
while (change < 0);
changeInt = round(change * 100);
printf("You're owed %i cents\n", changeInt);
}