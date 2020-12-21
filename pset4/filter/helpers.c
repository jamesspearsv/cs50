#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float blue = image[h][w].rgbtBlue;
            float green = image[h][w].rgbtGreen;
            float red = image[h][w].rgbtRed;

            float average = round((blue + green + red) / 3);

            image[h][w].rgbtBlue = average;
            image[h][w].rgbtGreen = average;
            image[h][w].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < (width / 2); w++)
        {
            //Store RGB values from current location in temp
            int tempBlue = image[h][w].rgbtBlue;
            int tempGreen = image[h][w].rgbtGreen;
            int tempRed = image[h][w].rgbtRed;

            //Move pixel from new location to current location
            image[h][w].rgbtBlue = image[h][(width - 1) - w].rgbtBlue;
            image[h][w].rgbtGreen = image[h][(width - 1) - w].rgbtGreen;
            image[h][w].rgbtRed = image[h][(width - 1) - w].rgbtRed;


            //Replace new location with temp RGB values
            image[h][(width - 1) - w].rgbtBlue = tempBlue;
            image[h][(width - 1) - w].rgbtGreen = tempGreen;
            image[h][(width - 1) - w].rgbtRed = tempRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            temp[h][w] = image[h][w];
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int sumBlue = 0;
            int sumGreen = 0;
            int sumRed = 0;
            float counter = 0;

            //Interates through smaller grid around current pixel
            for (int i = -1; i < 2; i++)
            {
                //Checks if adjacent pixel is outside image vertically
                if (h + i < 0 || h + i > height - 1)
                {
                    continue;
                }

                for (int j = -1; j < 2; j++)
                {
                    //Checks if adjacent pixel is outside image horizontally
                    if (w + j < 0 || w + j > width - 1)
                    {
                        continue;
                    }

                    sumBlue += temp[h + i][w + j].rgbtBlue;
                    sumGreen += temp[h + i][w + j].rgbtGreen;
                    sumRed += temp[h + i][w + j].rgbtRed;
                    counter++;
                }
            }

            image[h][w].rgbtBlue = round(sumBlue / counter);
            image[h][w].rgbtGreen = round(sumGreen / counter);
            image[h][w].rgbtRed = round(sumRed / counter);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            temp[h][w] = image[h][w];
        }
    }

    //Iterates through each pixel in temp[][]
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w ++)
        {
            int GxBlue = 0;
            int GxGreen = 0;
            int GxRed = 0;

            int GyBlue = 0;
            int GyGreen = 0;
            int GyRed = 0;

            //Iterates through subgrid around current pixel location
            for (int i = -1; i < 2; i++) //Row -> h
            {
                for (int j = -1; j < 2; j++) //Column -> w
                {
                    //Checks if adjacent pixel is outside image
                    if (h + i < 0 || h + i > height - 1 || w + j < 0 || w + j > width - 1)
                    {
                        GxBlue += 0;
                        GxGreen += 0;
                        GxRed += 0;

                        GyBlue += 0;
                        GyGreen += 0;
                        GyRed += 0;
                        continue;
                    }

                    //Subgrid location -1,-1
                    if (i == -1 && j == -1)
                    {
                        GxBlue += -1 * temp[h + i][w + j].rgbtBlue;
                        GxGreen += -1 * temp[h + i][w + j].rgbtGreen;
                        GxRed += -1 * temp[h + i][w + j].rgbtRed;

                        GyBlue += -1 * temp[h + i][w + j].rgbtBlue;
                        GyGreen += -1 * temp[h + i][w + j].rgbtGreen;
                        GyRed += -1 * temp[h + i][w + j].rgbtRed;
                    }

                    //Subgrid location -1,0
                    if (i == -1 && j == 0)
                    {
                        GxBlue += 0 * temp[h + i][w + j].rgbtBlue;
                        GxGreen += 0 * temp[h + i][w + j].rgbtGreen;
                        GxRed += 0 * temp[h + i][w + j].rgbtRed;

                        GyBlue += -2 * temp[h + i][w + j].rgbtBlue;
                        GyGreen += -2 * temp[h + i][w + j].rgbtGreen;
                        GyRed += -2 * temp[h + i][w + j].rgbtRed;
                    }

                    //Subgrid location -1,1
                    if (i == -1 && j == 1)
                    {
                        GxBlue += 1 * temp[h + i][w + j].rgbtBlue;
                        GxGreen += 1 * temp[h + i][w + j].rgbtGreen;
                        GxRed += 1 * temp[h + i][w + j].rgbtRed;

                        GyBlue += -1 * temp[h + i][w + j].rgbtBlue;
                        GyGreen += -1 * temp[h + i][w + j].rgbtGreen;
                        GyRed += -1 * temp[h + i][w + j].rgbtRed;
                    }

                    //Subgrid location 0,-1
                    if (i == 0 && j == -1)
                    {
                        GxBlue += -2 * temp[h + i][w + j].rgbtBlue;
                        GxGreen += -2 * temp[h + i][w + j].rgbtGreen;
                        GxRed += -2 * temp[h + i][w + j].rgbtRed;

                        GyBlue += 0 * temp[h + i][w + j].rgbtBlue;
                        GyGreen += 0 * temp[h + i][w + j].rgbtGreen;
                        GyRed += 0 * temp[h + i][w + j].rgbtRed;
                    }

                    //Subgrid location 0,0
                    if (i == 0 && j == 0)
                    {
                        GxBlue += 0 * temp[h + i][w + j].rgbtBlue;
                        GxGreen += 0 * temp[h + i][w + j].rgbtGreen;
                        GxRed += 0 * temp[h + i][w + j].rgbtRed;

                        GyBlue += 0 * temp[h + i][w + j].rgbtBlue;
                        GyGreen += 0 * temp[h + i][w + j].rgbtGreen;
                        GyRed += 0 * temp[h + i][w + j].rgbtRed;
                    }

                    //Subgrid location 0,1
                    if (i == 0 && j == 1)
                    {
                        GxBlue += 2 * temp[h + i][w + j].rgbtBlue;
                        GxGreen += 2 * temp[h + i][w + j].rgbtGreen;
                        GxRed += 2 * temp[h + i][w + j].rgbtRed;

                        GyBlue += 0 * temp[h + i][w + j].rgbtBlue;
                        GyGreen += 0 * temp[h + i][w + j].rgbtGreen;
                        GyRed += 0 * temp[h + i][w + j].rgbtRed;
                    }

                    //Subgrid location 1,-1
                    if (i == 1 && j == -1)
                    {
                        GxBlue += -1 * temp[h + i][w + j].rgbtBlue;
                        GxGreen += -1 * temp[h + i][w + j].rgbtGreen;
                        GxRed += -1 * temp[h + i][w + j].rgbtRed;

                        GyBlue += 1 * temp[h + i][w + j].rgbtBlue;
                        GyGreen += 1 * temp[h + i][w + j].rgbtGreen;
                        GyRed += 1 * temp[h + i][w + j].rgbtRed;
                    }

                    //Subgrid location 1,0
                    if (i == 1 && j == 0)
                    {
                        GxBlue += 0 * temp[h + i][w + j].rgbtBlue;
                        GxGreen += 0 * temp[h + i][w + j].rgbtGreen;
                        GxRed += 0 * temp[h + i][w + j].rgbtRed;

                        GyBlue += 2 * temp[h + i][w + j].rgbtBlue;
                        GyGreen += 2 * temp[h + i][w + j].rgbtGreen;
                        GyRed += 2 * temp[h + i][w + j].rgbtRed;
                    }

                    //Subgrid location 1,1
                    if (i == 1 && j == 1)
                    {
                        GxBlue += 1 * temp[h + i][w + j].rgbtBlue;
                        GxGreen += 1 * temp[h + i][w + j].rgbtGreen;
                        GxRed += 1 * temp[h + i][w + j].rgbtRed;

                        GyBlue += 1 * temp[h + i][w + j].rgbtBlue;
                        GyGreen += 1 * temp[h + i][w + j].rgbtGreen;
                        GyRed += 1 * temp[h + i][w + j].rgbtRed;
                    }
                }
            }

            //Calculate new RGB values
            int newBlue = round(sqrt((pow(GxBlue, 2) + pow(GyBlue, 2))));
            int newGreen = round(sqrt((pow(GxGreen, 2) + pow(GyGreen, 2))));
            int newRed = round(sqrt((pow(GxRed, 2) + pow(GyRed, 2))));

            //Cap new RGB values at 255
            if (newBlue > 255)
            {
                newBlue = 255;
            }

            if (newGreen > 255)
            {
                newGreen = 255;
            }

            if (newRed > 255)
            {
                newRed = 255;
            }

            //Assign new RGB values to current pixel location in image[][]
            image[h][w].rgbtBlue = newBlue;
            image[h][w].rgbtGreen = newGreen;
            image[h][w].rgbtRed = newRed;
        }
    }
    return;
}