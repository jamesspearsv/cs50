#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void cipher(string text, int key);

int main(int argc, string argv[])
{
    if (argc != 2) //Checks that user has included key argument
    {
        printf("Usage: ./caesar <key>\n");
        return 1;
    }
    else if (argc == 2) //Checks that each char is a digit
    {
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (isdigit(argv[1][i]) == 0)
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

    cipher(plainText, intKey);

    return 0;
}

void cipher(string s, int key)
{
    string cipher = s;

    //ci = (pi + k) % 26
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
        {
            cipher[i] = (((s[i] - 'A') + key) % 26) + 'A';
        }
        else if (s[i] >= 'a' && s[i] <= 'z')
        {
            cipher[i] = (((s[i] - 'a') + key) % 26) + 'a';
        }
        else cipher[i] = s[i];
    }
    printf("%s\n", cipher);
}