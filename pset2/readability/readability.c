#include <stdio.h>
#include <cs50.h>
#include <string.h>

int countLetters(string s);
int countWords(string s);
int countSentences(string s);
float readingLevel(float letters, float words, float sentences);

int main(void)
{
    string text = get_string("Text: ");
    //Gets necessary values from text: index[0] = letters, index[1] = words, index[2] = sentences
    int index[3] = {countLetters(text), countWords(text), countSentences(text)};
    float gradeLevel = readingLevel(index[0], index[1], index[2]);

    //Compared returned reading level and outputs to user
    if (gradeLevel < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (gradeLevel > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", gradeLevel);
    }

}

//Counts the number of letters in a string
int countLetters(string s)
{
    int letterCount = 0;
    int n = strlen(s);

    for (int i = 0; i < n; i++)
    {
        //Stores inth char in ASCII
        int ascii = s[i];
        if ((ascii > 64 && ascii < 91) || (ascii > 96 && ascii < 123)) //A -> Z and a -> z
        {
            letterCount += 1;
        }
    }
    return letterCount;
}

//Counts the number of words in string
int countWords(string s)
{
    int wordCount = 0;
    int n = strlen(s);

    for (int i = 0; i <= n; i++)
    {
        //Stores inth char in ASCII
        int ascii = s[i];
        //Counts words in string
        if (ascii == 32 || ascii == 0) //SPACE(32) -> !(33) -> .(46) -> ?(63)
        {
            wordCount += 1;
        }
    }
    return wordCount;
}

//Counts number of sentence in string
int countSentences(string s)
{
    int sentenceCount = 0;
    int n = strlen(s);

    for (int i = 0; i < n; i++)
    {
        //Stores inth char in ASCII
        int ascii = s[i];
        //Counts words in string
        if (ascii == 33 || ascii == 46 || ascii == 63) //!(33) -> .(46) -> ?(63)
        {
            sentenceCount += 1;
        }
    }
    return sentenceCount;
}

//Calculates Coleman-Liau index: index = 0.0588 * L - 0.296 * S - 15.8
float readingLevel(float letters, float words, float sentences)
{
    //Average letters per 100 words
    float l = (letters / words) * 100;
    //average sentences per 100 words
    float s = (sentences / words) * 100;
    return (0.0588 * l) - (0.296 * s) - 15.8;
}