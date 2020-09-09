#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef uint8_t BYTE;
int imgCount = 0; //count how many images have been recovered
char imgName[7];
FILE *img;

int main(int argc, char *argv[])
{
    //check for invalid usage
    if (argc != 2)
    {
        printf("Usage: recover [input]\n");
        return 1;
    }

    //open file to recover and name it rawData
    FILE *file = fopen(argv[1], "r");

    //define jpeg block size
    int blockSize = 512;//sizeof(int);
    BYTE buffer[blockSize]; //creates a buffer of 512 bytes

    //read file in 512 bit chunks
    while (fread(buffer, blockSize, 1, file) == 1)
    {
        //check to see if we are at the start of a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (imgCount == 0)
            {
                sprintf(imgName, "%03i.jpg", imgCount);
                img = fopen(imgName, "w");
                fwrite(buffer, blockSize, 1, img);
            }
            else
            {
                fclose(img);
                sprintf(imgName, "%03i.jpg", imgCount);
                img = fopen(imgName, "w");
                fwrite(buffer, blockSize, 1, img);
            }

            imgCount++;

        }
        else if (imgCount != 0)
        {
            fwrite(buffer, blockSize, 1, img);
        }
    }

    fclose(img);
    fclose(file);

    return 0;
}
