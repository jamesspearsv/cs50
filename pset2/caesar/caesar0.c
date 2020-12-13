#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//bool validateKey(string s);

int main(int argc, string argv[])
{
    if (argc != 2) //Checks that user has included key argument
    {
        printf("Usage: ./caesar <key>\n");
        return 1;
    }
    else if (argc == 2)
    {
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (isdigit(argv[1][i]) == 0) //Checks that each char is a digit
            {
                printf("Numeric characters only please.\n");
                return 1;
                break;
            }
        }
    }

    int intKey = atoi(argv[1]);
    string plainText = get_string("plaintext: ");
    printf("ciphertext: ");

    string cipherText = plainText;

    for (int j = 0; j < strlen(cipherText); j++)
    {
        cipherText[j] += intKey;
    }
    printf("%s\n", cipherText);

    return 0;
}