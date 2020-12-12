#include <stdio.h>
#include <cs50.h>

int main(void)
{
    char x = 'a';

    while (x != 'x')
    {
        printf("Enter x to exit program\n");
        x = get_char("Enter a character: ");
        printf("Character '%c' in ASCII is %i\n", x, x);
        printf("\n");
    }
}