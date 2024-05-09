#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *file_name1, *file_name2;
    int ans_flag = 0, case_sen_flag = 0;

    // Check how many command-line arguments inputted - need to be 4 or 5
    if (argc != 4 && argc != 5) {

        fprintf(stderr, "Usage: %s <file1> <file2> [-v] [-i]\n", argv[0]);
        return 1;
    }

    // Insert the input names of files
    file_name1 = argv[1];
    file_name2 = argv[2];

    /* Set the flags */
    if (argc == 5)
    {
        if (((strcmp(argv[3], "-v")) == 0 && (strcmp(argv[4], "-i") == 0)) ||
        ((strcmp(argv[3], "-i") == 0) && (strcmp(argv[4], "-v") == 0)))
        {
            ans_flag = 1;
            case_sen_flag = 1;
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
        else if (strcmp(argv[3], "-i") == 0) case_sen_flag = 1;
        else
        {
            fprintf(stderr, "Unknown flag: %s\n", argv[3]);
            return 1;
        }
    }

    /* Open the files */
    FILE *file1 = fopen(file_name1, "rb");
    if (file1 == NULL) {
        fprintf(stderr, "Can't open file: %s\n", file_name1);
        return 1;
    }

    FILE *file2 = fopen(file_name2, "rb");
    if (file2 == NULL) {
        fprintf(stderr, "Can't open file: %s\n", file_name2);
        fclose(file1);

        return 1;
    }

    /* Compare the files */
    int is_equal = 1;
    int byte1, byte2;

    do {
        byte1 = fgetc(file1);
        byte2 = fgetc(file2);

        if (case_sen_flag && byte1 >= 'A' && byte1 <= 'Z') {
            byte1 += 'a' - 'A';
        }

        if (case_sen_flag && byte2 >= 'A' && byte2 <= 'Z') {
            byte2 += 'a' - 'A';
        }

        if (byte1 != byte2) {
            is_equal = 0;
            break;
        }

    } while (byte1 != EOF && byte2 != EOF);

    // Close the files
    fclose(file1);
    fclose(file2);

    /* Print the result */
    if (is_equal) {
        if (ans_flag) {
            printf("equal\n");
        }
        return 0;
    } 
else {
        if (ans_flag) {
            printf("distinct\n");
        }
        return 1;
    }
}