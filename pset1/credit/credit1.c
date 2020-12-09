#include <stdio.h>
#include <cs50.h>
#include <math.h>

void checkSum(long creditNum);

int pos = 0, sumEven = 0, sumOdd = 0;

int main(void)
{
    long credit = get_long("Number: ");
    checkSum(credit);
}


//reduces and indexes credit card number
void checkSum(long creditNum)
{
  while (creditNum != 0)
  {
      int digit = creditNum % 10;

      if (pos % 2 == 0)
      {
          printf("Position (%i): %i\n",pos, digit);
      }
      else
      {
          printf("Position (%i): %i\n",pos, digit);
      }
      creditNum = creditNum / 10;
      pos++;
  }
}