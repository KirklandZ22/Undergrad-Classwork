// COP3502 Spring 2022
// Assingment 1: Numeronym

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Value of when a word is long enough to be shortened
#define LONG_ENOUGH 9

void printNumeronym(char *str);
void printShortenedSentence(char *str);
double difficultyRating(void);
double hoursSpent(void);

int main(int argc, char **argv)
{
    // i used in for loop, start i at one as the 0 string is the execution program
    int i;
    for (i = 1; i < argc; i++)
    {
        printNumeronym(argv[i]);
    }

}

void printNumeronym(char *str)
{
    // set len to the length of string
    int len = strlen(str);
    // if the length is not long enough to be shortened print no change condition
    if (len < LONG_ENOUGH)
    {
        printf("%s (no change)\n", str);
        return;
    }
    // if the length is long enough to be shortened then
    // print the string conversion of the first char, the length - 2, the last char
    else
    {
        printf("%s -> %c%d%c\n", str, str[0], len - 2, str[len - 1]);
        return;
    }
}

void printShortenedSentence(char *str)
{
    // i and j are loop values, wlen holds the lenght of the current word, slen holds the lenght of the string
    int i = 0, j, wlen = 0, slen = strlen(str);
    while (i < slen)
    {
        // while the char in the string is an alphebetic character
        while (isalpha(str[i]) != 0)
        {
            // increase word len and i
            wlen++;
            i++;
        }
        // once at a nonalphebetic character
        // if word lenght is not long enough to be shortened print the word
        if (wlen < LONG_ENOUGH)
        {
            for (j = wlen; j > 0; j--)
            {
                printf("%c", str[i - j]);
            }
        }
        //if word lenght is long enough to be shortened then
        //print the string conversion of the first char, the length - 2, the last char
        else
        {
            // i - wlen goes to first character
            printf("%c%d%c", str[i - wlen], wlen - 2, str[i - 1]);
        }
        // print the character that is currently i (either space or punctuation)
        printf("%c", str[i]);
        //reset word length and increase i
        wlen = 0;
        i++;
    }
    //print new line to end sentence
    printf("\n");
}


double difficultyRating(void)
{
    return 2.5;
}

double hoursSpent(void)
{
    return 3.25;
}
