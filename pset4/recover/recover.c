#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover <infile>\n");
        return 1;
    }

    //Tracking variables
    int fileCounter = -0;
    int jpgFound = 0; //False

    //Buffer, fileName, outFile
    unsigned char buffer[512];
    //char fileName[8]; -> file name in an array of chars
    char *fileName = malloc(8 * sizeof(char));
    FILE *img = NULL;

    //Open card
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Unable to read file\n");
        return 1;
    }

    //Read 512 Bytes into buffer while blocks remain to be read
    while (fread(buffer, 512, 1, input) == 1)
    {
        //If start of new new JPEG
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            //If second and so on JPEG
            if (jpgFound == 1)
            {
                //Close current img and free memory
                fclose(img);
                free(fileName);

                //Re-allocate memory for new file name
                fileName = malloc(8 * sizeof(char));
            }

            //If fisst JPEG of infile
            if (jpgFound == 0)
            {
                //set jpgFound to 1
                jpgFound = 1;
            }

            //Name, open, and write to new jpg file
            sprintf(fileName, "%03i.jpg", fileCounter);
            img = fopen(fileName, "w");
            fwrite(buffer, 512, 1, img);
            fileCounter++;
        }

        //if block is not start of JPEG
        else if (jpgFound == 1)
        {
            //Write to current open jpg file
            fwrite(buffer, 512, 1, img);
        }
    }

    //Close all remaining files
    if (input == NULL)
    {
        fclose(input);
    }

    if (img == NULL)
    {
        fclose(img);
    }

    free(fileName);
    return 0;
}