#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // assign infile to variable
    char *infile = argv[1];

    // open infile
    FILE *raw_file = fopen(infile, "r");

    // check if infile is null

    if (raw_file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // define variables
    int NumJpeg = 0;
    char filename[8];
    unsigned char buffer[512];

    // outfile
    FILE *outfile = NULL;

    // check for end of file
    while (fread(buffer, 512, 1, raw_file) == 1)
    {
        // check start of file
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // write to new jpeg if first jpeg
            if (NumJpeg == 0)
            {
                sprintf(filename, "%03i.jpg", NumJpeg);
                NumJpeg++;
                outfile = fopen(filename, "w");
                fwrite(buffer, 512, 1, outfile);
            }

            // check if a jpeg was previously found
            else if (NumJpeg != 0)
            {
                fclose(outfile);
                sprintf(filename, "%03i.jpg", NumJpeg);
                NumJpeg++;
                outfile = fopen(filename, "w");
                fwrite(buffer, 512, 1, outfile);
            }

        }

        // continue writing jpeg
        else if (NumJpeg != 0)
        {
            fwrite(buffer, 512, 1, outfile);
        }
    }

    int read = fread(buffer, 1, 512, raw_file);
    fwrite(buffer, 1, read, outfile);
    fclose(outfile);

    return 0;
}
