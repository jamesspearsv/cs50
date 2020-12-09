#include <stdio.h>
#include <cs50.h>

int main(void) {
	//prompts user for number, check if between 1 and 8
	int height;
	do {
	    height = get_int("Height: ");
	}
	while (height < 1 || height > 8);
    
    //returns value of var height
	printf("stored: %i\n", height);
}