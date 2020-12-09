#include <stdio.h>
#include <cs50.h>

int getPositiveInt(void);

int main(void) {

    int height = getPositiveInt();
    printf("stored: %i\n", height);

    //Prints game piece to screen
	for (int i = 1; i <= height; i++) {
	    for (int j = 1; j <= height; j++) {
	        printf("#");
	    }
	    printf("\n");
	}
}












//Function to get positive int from user
int getPositiveInt(void) {
    int n;
	do {
	    n = get_int("Height: ");
	}
	while (n < 1 || n > 8);
	return n;
}