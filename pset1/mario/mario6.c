#include <stdio.h>
#include <cs50.h>

int getPositiveInt(void);
void printPiece(int x);

int main(void) {

	//Gets user input
	int height = getPositiveInt();
	printf("Stored: %i\n", height);

    //print game piece
    printPiece(height);
}



//Function to get positive int from user
int getPositiveInt(void) {
	int n;
	do {
		n = get_int("Height: ");
	}
	while (n < 1 || n > 50);
	return n;
}

//Function to print game piece
void printPiece(int x) {
    //Prints game piece to screen
	//Controls newLine break
	for (int i = x; i >= 1; i--) {
		//Prints # in horizontal direction
		for (int j = 0; j <= x; j++) {
			//Controls what is printed
			if (j > i -1) {
			    printf("#");
			}
		    else {
		        printf(" ");
		    }
		}
		printf("\n");
	}
}