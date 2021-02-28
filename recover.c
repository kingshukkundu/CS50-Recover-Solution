#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
#define FILENAME_MEM 8

bool is_jpg_header(uint8_t read[BLOCK_SIZE]);

int main(int argc, char *argv[])
{
    //Check for proper command line arguemnts
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 2;
    }
    uint8_t buffer[BLOCK_SIZE];
    bool first_jpg = true;
    char filename[FILENAME_MEM];
    int count = 0;
    FILE *output = NULL;

    //iterate through all the blocks until the end of file
    while (fread(buffer, BLOCK_SIZE, 1, input))
    {
        //if valid header, start writing jpg
        if (is_jpg_header(buffer))
        {
            if (first_jpg)
            {
                first_jpg = false;
            }
            //if not first jpg close previous write file
            else
            {
                fclose(output);
            }
            //update filename accoring to count
            sprintf(filename, "%03i.jpg", count++);
            //open filename in write mode
            output = fopen(filename, "w");
            if (output == NULL)
            {
                return 3;
            }
            //start writing to filenmae.jpg
            fwrite(buffer, BLOCK_SIZE, 1, output);
        }
        //if not valid header and not first jpeg continue writing to previous jpeg
        else if (!first_jpg)
        {
            fwrite(buffer, BLOCK_SIZE, 1, output);
        }
    }
    fclose(output);
}

//Check for valid JPEG header
bool is_jpeg_header(unsigned char header[])
{
    return (header[0] == SOI_0 &&
            header[1] == SOI_1 &&
            header[2] == APPN &&
            ((header[3] & 0xf0) == 0xe0));
}
