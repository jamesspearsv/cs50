#include <stdio.h>
#include <cs50.h>
#include <math.h>

bool checkSum(long creditNum);
int issuerCheck(long creditNum);

int main(void)
{
    //Prompts user for credit card number
    long credit = get_long("Number: ");
    //Runs Luhn’s algorithm and checks card issuer
    bool valid = checkSum(credit);
    int issuer = issuerCheck(credit);

    //Returns to user card type or invalid
    if (issuer == 1 && valid == true)
    {
        printf("VISA\n");
    }
    else if (issuer == 2 && valid == true)
    {
        printf("AMEX\n");
    }
    else if (issuer == 3 && valid == true)
    {
       printf("MASTERCARD\n");
    }
    else
    {
       printf("INVALID\n");
    }

}


//runs check sum to validate credit card number
bool checkSum(long creditNum)
{
int pos = 0, sumEven = 0, sumOdd = 0, tempDigit = 0;
  //Runs Luhn’s algorithm
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
//Gets total sum and check that trailing digit is 0
int totalSum = sumOdd + sumEven;
if (totalSum % 10 == 0)
{
    return true;
}
else
{
    return false;
}
}

//Checks issuer of a credit card number
int issuerCheck(long creditNum)
{
    int numLength = 0;
    long tempCredit = creditNum;



    //Counts card number length
    while (creditNum != 0)
    {
        int temp = creditNum % 10;
        creditNum = creditNum / 10;
        numLength++;
    }

    //Find leading digit for each card type and length
    long exponent = numLength - 2;
    long lengthDivisor = powl(10, exponent);
    int leadingDigit = tempCredit / lengthDivisor;

    //Check constraints to determine card issuer
    if ((numLength == 13 || numLength == 16) && (leadingDigit >= 40 && leadingDigit <= 49))
    {
        //VISA
        return 1;
    }
    else if (numLength == 15 && (leadingDigit == 34 || leadingDigit == 37))
    {
        //AMEX
        return 2;
    }
    else if (numLength == 16 && (leadingDigit >= 51 && leadingDigit <= 55))
    {
        //MASTERCARD
        return 3;
    }
    else
    {
        //Invalid
        return 4;
    }
}