#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *src_filename, *dest_filename;
    int ans_flag = 0, force_flag = 0;

    // Check how many command-line arguments inputted - need to be 4 or 5
    if (argc != 4 && argc != 5) {
        fprintf(stderr, "Usage: %s <src_file> <dest_file> [-v] [-f]\n", argv[0]);
        return 1;
    }

    // Insert the input names of files
    src_filename = argv[1];
    dest_filename = argv[2];

    /* Set the flags */
    if (argc == 5)
    {
        if (((strcmp(argv[3], "-v")) == 0 && (strcmp(argv[4], "-f") == 0)) ||
            ((strcmp(argv[3], "-f") == 0) && (strcmp(argv[4], "-v") == 0)))
        {
            ans_flag = 1;
            force_flag = 1;
        }
        else
        {
            fprintf(stderr, "Unknown flag\n");
            return 1;
        }
    }
    else // Has to be: argc = 4
    {
        if (strcmp(argv[3], "-v") == 0) ans_flag = 1;
        else if (strcmp(argv[3], "-f") == 0) force_flag = 1;
        else
        {
            fprintf(stderr, "Unknown flag: %s\n", argv[3]);
            return 1;
        }
    }

    /* Open the source file */
    FILE *src_file = fopen(src_filename, "rb");

    if (src_file == NULL) {
        fprintf(stderr, "Can't open file: %s\n", src_filename);
        return 1;
    }

    /* Check if the destination file exists */
    FILE *dest_file = fopen(dest_filename, "rb");

    if (dest_file != NULL)
    {
        if (!force_flag)
        {
            if (ans_flag) printf("target file exist\n");

            fclose(src_file);
            fclose(dest_file);

            return 1;
        }
        fclose(dest_file);
    }

    /* Open the destination file for writing */
    dest_file = fopen(dest_filename, "wb");

    if (dest_file == NULL) {
        fprintf(stderr, "Can't create file: %s\n", dest_filename);
        fclose(src_file);

        return 1;
    }

    /* Copy the data of the source file into destination file */
    char buf[1024];
    int bytes;

    while ((bytes = fread(buf, 1, sizeof(buf), src_file)) > 0) {
        if (fwrite(buf, 1, bytes, dest_file) != bytes) {
            fprintf(stderr, "Error occurred while writing to file: %s\n", dest_filename);
            fclose(src_file);
            fclose(dest_file);
            remove(dest_filename);  // Delete the partially copied file

            return 1;
        }
    }

    // Close the files
    fclose(src_file);
    fclose(dest_file);

    // Print the result
    if (ans_flag) printf("success\n");

    return 0;
}