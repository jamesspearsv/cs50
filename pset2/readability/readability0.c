#include <stdio.h>
#include <cs50.h>
#include <string.h>

void readability(string);
int countLetters (string s);

int main(void)
{
    //Take input from user
    string text = get_string("Text: ");
    printf("%i letter(s)\n", countLetters(text));

}

void readability(string text)
{
    //Count number of letters in text
    int stringLength = strlen(text);
    printf("stringLength: %i\n", stringLength);
    //Calculate L value
    //Calculate S value
    //Complete Coleman-Liau index: index = 0.0588 * L - 0.296 * S - 15.8
    //Return grade value to user
}

int countLetters (string s)
{
    int letterNum = 0;
    int n = strlen(s);

    for (int i = 0; i < n; i++) {
        int ascii = s[i];
        if ((ascii > 96 && ascii < 123) || (ascii > 64 && ascii < 91))
        {
            letterNum += 1;
        }
        else
        {
            letterNum += 0;
        }
    }
    return letterNum;
}