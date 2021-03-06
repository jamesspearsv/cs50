#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover <infile>\n");
        return 1;
    }

    //Tracking variables
    int fileCounter = 0;
    bool jpgFound = false;

    //Buffer, outFile, and fileName
    unsigned char buffer[512];
    FILE *img = NULL;
    char *fileName = malloc(8 * sizeof(char));
    if (fileName == NULL)
    {
        return 2; //Memory error
    }

    //Open card
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Unable to read file\n");
        return 1;
    }

    //Read 512 Bytes into buffer while not at end of file
    while (fread(buffer, 512, 1, input) != EOF)
    {
        //If start of new new JPEG
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            //If second and so on JPEG
            if (jpgFound == true)
            {
                //Close current open img
                fclose(img);
            }

            //If fisst JPEG of infile
            if (jpgFound == false)
            {
                //set jpgFound to 1
                jpgFound = true;
            }

            //Name, open, and write to new jpg file
            sprintf(fileName, "%03i.jpg", fileCounter);
            img = fopen(fileName, "w");
            fwrite(buffer, 512, 1, img);
            fileCounter++;
        }

        //if block is not start of JPEG
        else if (jpgFound == true)
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
