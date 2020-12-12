//BROKEN. CHECK READABILITY2.C

#include <stdio.h>
#include <cs50.h>
#include <string.h>

int counter (string s);

int main(void)
{
    //Take input from user
    string text = get_string("Text: ");
    //printf("%i letter(s)\n",
    counter(text);

}

int counter (string s) //Counter the number of letters in a string.
{
    //int letterCount = 0;
    //int wordCount = 0;
    //int sentenceCount = 0;
    int dataArray[3]; //pos0 = letterCount; pos1 = wordCount; pos2 = sentenceCount
    int n = strlen(s);

    for (int i = 0; i < n; i++) {
        //Stores inth char in ASCII
        int ascii = s[i];

        //Counts letters in string
        if ((ascii > 64 && ascii < 91) || (ascii > 96 && ascii < 123)) //A -> Z and a -> z
        {
            dataArray[0] += 1;
        }
        //Counts words in string
        else if (ascii == 32) //SPACE(32)
        {
            dataArray[1] += 1;
        }
        //Counts sentences in string
        else if (ascii == 33 || ascii == 46 || ascii == 63) //!(33) -> .(46) -> ?(63)
        {
            dataArray[1] += 1;
            dataArray[2] += 1;
        }
    }
    printf("%i letter(s)\n%i word(s)\n%i sentence(s)\n", dataArray[0], dataArray[1], dataArray[2]);
    return dataArray[];
}