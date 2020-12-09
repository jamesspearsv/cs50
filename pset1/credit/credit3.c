#include <stdio.h>
#include <cs50.h>
#include <math.h>

bool checkSum(long creditNum);
void issuerCheck(long creditNum);

int main(void)
{
    long credit = get_long("Number: ");
    bool valid = checkSum(credit);
    int issuer = issuerCheck(credit);

    if (valid == false)
   {
        printf("INVALID\n");
    }
    else if (issuer == 1)
    {
        printf("VISA\n");
   }
    else if (issuer == 2)
    {
        printf("AMERICAN EXPRESS\n");
    }
  else
   {
       printf("MASTERCARD\n");
   }

}


//runs check sum to validate credit card number
bool checkSum(long creditNum)
{
int pos = 0, sumEven = 0, sumOdd = 0, tempDigit = 0;
  while (creditNum != 0)
  {
      int indexDigit = creditNum % 10;

      //Even numbers; beginning from the end of creditNum
      if (pos % 2 == 0)
      {
          sumEven += indexDigit;
      }
      //Odd numbers; beginning from penultimate number in creditNum. Multiplied by 2
      else
      {
        tempDigit = indexDigit * 2;

       if (tempDigit > 9)
       {
           while (tempDigit != 0)
           {
               int temp = 0;
               temp = temp + (tempDigit % 10);
               sumOdd += temp;
               tempDigit = tempDigit / 10;
           }
       }
       else
       {
           sumOdd += tempDigit;
       }
      }
      creditNum = creditNum / 10;
      pos++;
  }
int totalSum = sumOdd + sumEven;
if (totalSum % 10 !== 0)
{
    return false;
}
}

int issuerCheck(long creditNum)
{
    int numLength = 0;

    int leadingDigit = creditNum / 1000000000000000;

    //Counts card number length
    while (creditNum != 0)
    {
        int temp = creditNum % 10;
        creditNum = creditNum / 10;
        numLength++;
    }

    if (numLength == 13 || leadingDigit == 4)
    {
        //VISA
        return 1;
    }
    else if (numLength == 15)
    {
        //AMERICAN EXPRESS
        return 2;
    }
    else
    {
        //MASTERCARD
        return 3;
    }
}