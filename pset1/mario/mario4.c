#include <stdio.h>
#include <cs50.h>

int getPositiveInt(void);

int main(void) {

	//Gets user input
	int height = getPositiveInt();
	printf("stored: %i\n", height);

	//Prints game piece to screen
	//Controls newLine break
	for (int i = 1; i <= height; i++) {
		//Prints # in horizontal direction
		for (int j = 1; j <= i; j++) {
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