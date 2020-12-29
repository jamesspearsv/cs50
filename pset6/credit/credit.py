from cs50 import get_int, get_string
from math import trunc


def main():
    cardNum = get_int("Card number: ")
    cSum = checkSum(cardNum)
    issuer = cardIssuer(cardNum)

    if cSum == True and issuer == 1:
        print("VISA")
    elif cSum == True and issuer == 2:
        print("AMEX")
    elif cSum == True and issuer == 3:
        print("MASTERCARD")
    else:
        print("INVALID")


def checkSum(cardNum):
    checkSum = sumOdd = sumEven = pos = temp = 0
    # calculates sum of odd and even position numbers
    # beginning from end of card number
    while cardNum >= 1:
        currentDigit = trunc(cardNum % 10)
        if pos % 2 == 0:
            sumEven += currentDigit
        else:
            temp = currentDigit * 2
            if temp > 9:
                n = 0
                while temp >= 1:
                    n = trunc((temp % 10)) + n
                    temp = temp / 10
                sumOdd += n
            else:
                sumOdd += temp
        cardNum = cardNum / 10
        pos += 1

    # Checks final digit of checkSum. If 0 -> Valid card num
    checkSum = sumOdd + sumEven
    if checkSum % 10 == 0:
        return True
    else:
        return False


def cardIssuer(cardNum):
    numLen = leadingNum = 0
    temp = cardNum

    while temp != 0:
        temp = trunc(temp / 10)
        numLen += 1
    leadingNum = trunc(cardNum / (10 ** (numLen - 2)))

    if (numLen == 13 or numLen == 16) and (leadingNum >= 40 and leadingNum <= 49):
        # It's a VISA
        return 1
    elif (numLen == 15) and (leadingNum == 34 or leadingNum == 37):
        # It's a American Express
        return 2
    elif (numLen == 16) and (leadingNum >= 51 and leadingNum <= 55):
        # It's a Master Card
        return 3
    else:
        # It's invalid
        return 4


main()